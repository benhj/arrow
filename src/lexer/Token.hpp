/// (c) Ben Jones 2019 - present

#pragma once

#include "Lexeme.hpp"
#include <string>
#include <utility>

namespace arrow {
    struct Token {

        Token()
        : lexeme(Lexeme::NIL)
        , raw("")
        , lineNumber(0)
        {
        }

        Token(Lexeme lex, std::string str, long const ln) 
        : lexeme(std::move(lex))
        , raw(std::move(str))
        , lineNumber(ln)
        {
        }

        Token(Token const & other)
          : lexeme(other.lexeme)
          , raw(other.raw)
          , lineNumber(other.lineNumber)
        {
        }

        Lexeme lexeme;
        std::string raw;
        long lineNumber;
    };
}