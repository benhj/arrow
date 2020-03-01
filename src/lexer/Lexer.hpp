/// (c) Ben Jones 2019 - present

#pragma once

#include "Token.hpp"
#include <iostream>
#include <vector>

namespace arrow {
    class Lexer {

      public:
        static std::vector<Token> tokenize(std::istream & stream);
        static std::vector<Token> tokenizeProgArgs(std::istream & stream);

    };
}