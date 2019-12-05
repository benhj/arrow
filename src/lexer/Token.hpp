/// (c) Ben Jones 2019 - present

#pragma once

#include "Lexeme.hpp"
#include <string>
#include <utility>

namespace arrow {
    struct Token {

        Token(){}

        Token(Lexeme lex, std::string str, long const ln) 
        : lexeme(std::move(lex))
        , raw(std::move(str))
        , lineNumber(ln)
        {
        }

        Lexeme lexeme;
        std::string raw;
        long lineNumber;
    };
}