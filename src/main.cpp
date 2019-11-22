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

/*
    for(auto const & token : tokens) {
        std::cout<<token.lexeme<<"\t"<<token.raw<<std::endl;
    }
*/
    arrow::Parser p(tokens);
    p.parse();
    auto statements = p.getStatements();

    for(auto const & s : statements) {
        s->getEvaluator()->evaluate(cache);
    }

    return 0;
}