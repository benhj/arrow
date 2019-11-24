#include "lexer/Lexer.hpp"
#include "lexer/Lexeme.hpp"
#include "parser/Parser.hpp"
#include "representation/Cache.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include <fstream>
#include <iostream>

int main(int argc, char ** argv) {

    std::ifstream in(argv[1]);
    arrow::Cache cache;
    auto tokens = arrow::Lexer::tokenize(in);
    try {
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