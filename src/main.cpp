#include "lexer/Lexer.hpp"
#include "lexer/Lexeme.hpp"
#include "parser/Parser.hpp"
#include <fstream>
#include <iostream>

int main(int argc, char ** argv) {

    std::ifstream in(argv[1]);

    auto tokens = jasl::Lexer::tokenize(in);

    for(auto const & token : tokens) {
        std::cout<<token.lexeme<<"\t"<<token.raw<<std::endl;
    }

    jasl::Parser p(tokens);
    p.parse();
    auto statements = p.getStatements();

    for(auto const & s : statements) {
        std::cout<<s->toString()<<std::endl;
    }

    return 0;
}