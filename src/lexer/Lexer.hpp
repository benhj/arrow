#pragma once

#include "Token.hpp"
#include <iostream>
#include <vector>

namespace jasl {
    class Lexer {

      public:
        static std::vector<Token> tokenize(std::istream & stream);

    };
}