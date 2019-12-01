#include "lexer/Lexer.hpp"
#include "lexer/Lexeme.hpp"
#include "parser/Parser.hpp"
#include "parser/LanguageException.hpp"
#include "representation/Cache.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

int main(int argc, char ** argv) {

    try {
        std::ifstream in(argv[1]);
        arrow::Cache cache;

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