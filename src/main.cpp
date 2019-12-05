/// Ben Jones 2019 - present

#include "lexer/Lexer.hpp"
#include "lexer/Lexeme.hpp"
#include "parser/Parser.hpp"
#include "parser/LanguageException.hpp"
#include "representation/Cache.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include "SimplePrompt.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

int main(int argc, char ** argv) {

    arrow::Cache cache;
    // Load program file when an argument is provided
    if(argc > 1) {
        try {
            std::ifstream in(argv[1]);

            // First parse program args
            if(argc > 2) {
                std::stringstream in;
                int i = 2;
                for(; i < argc - 1; ++i) {
                    in << argv[i] << " ";
                }
                in << argv[i];
                auto argTokens = arrow::Lexer::tokenize(in);
                arrow::Parser p(argTokens);
                auto progArgs = p.parseProgramArguments();
                for(auto const & a : progArgs) {
                    cache.pushProgramArgument(a);
                }
            }

            // Now parse actual program
            auto tokens = arrow::Lexer::tokenize(in);
            arrow::Parser p(tokens);
            p.parse();
            auto start = p.getStartStatement();
            if(start) {
                start->getEvaluator()->evaluate(cache);
            }
        } catch (std::exception const & e) {
            std::cout<<"\u001b[31;1mError: \u001b[0m";
            std::cout<<e.what()<<std::endl;
        }
        return 0;
    }
    // else run in interactive mode
    arrow::Parser p;
    simpleprompt::SimplePrompt sp("", [&](std::string const & com){
        if(com == "quit") {
            exit(0);
        }
        try {
            auto statements = p.getStatements();
            for(auto const & s : statements) {
                s->getEvaluator()->evaluate(cache);
            }
        } catch (arrow::LanguageException const & e) {
            std::cout<<"\u001b[31;1mError: \u001b[0m";
            std::cout<<e.what()<<std::endl;
        } catch (...) {
        }
    },
    [&](std::string const & str) {

        if(str == "quit") {
            std::cout<<"\033[1;37m"<<str;
        } else {
            if (str != "\n") {
                std::stringstream ss(str);
                try {
                    auto tokens = arrow::Lexer::tokenize(ss);
                    p.setTokens(std::move(tokens));
                    p.parse();
                    std::cout<<"\033[1;37m";
                } catch (...) {
                    std::cout<<"\033[1;31m";
                }
            }
            std::cout<<str;
            std::cout<<"\033[0m";
        }
    }, "Arrow v0.1", "\033[1;32m>> \033[0m");
    sp.start();


    return 0;
}