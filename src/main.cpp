/// Ben Jones 2019 - present

#include "lexer/Lexer.hpp"
#include "lexer/Lexeme.hpp"
#include "parser/ExpressionParser.hpp"
#include "parser/Parser.hpp"
#include "parser/LanguageException.hpp"
#include "representation/Environment.hpp"
#include "statements/evaluator/StatementEvaluator.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

int main(int argc, char ** argv) {

    arrow::Environment environment(std::cout);
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
                arrow::Parser p(argTokens, std::cout);
                auto progArgs = p.parseProgramArguments();
                for(auto const & a : progArgs) {
                    environment.pushProgramArgument(a);
                }
            }

            // Now parse actual program
            auto tokens = arrow::Lexer::tokenize(in);
            arrow::Parser p(tokens, std::cout);
            p.parse();
            auto start = p.getStartStatement();
            if(start) {
                start->getEvaluator()->evaluate(environment);
            }
        } catch (std::exception const & e) {
            std::cout<<"\u001b[31;1mError: \u001b[0m";
            std::cout<<e.what()<<std::endl;
        }
        return 0;
    }
    // else run in interactive mode
    std::cout<<"\nArrow v0.1\n\n"<<std::endl;
    while(true) {
        std::cout<<">> ";
        std::string com;
        std::getline (std::cin, com);
        if (com == "quit") {
            exit(0);
        }
        std::stringstream ss(com);
        auto tokens = arrow::Lexer::tokenize(ss);
        arrow::Parser parser(tokens, std::cout);
        try {
            parser.parse();
            auto statements = parser.getStatements();
            for(auto const & s : statements) {
                s->getEvaluator()->evaluate(environment);
            }
        } catch (arrow::LanguageException const & e) {
            std::cout<<e.report(false)<<std::endl;
        }
    }
    return 0;
}