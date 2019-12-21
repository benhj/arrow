/// (c) Ben Jones 2019 - present

#pragma once

#include "lexer/Token.hpp"
#include <vector>

namespace arrow {

    class TokenManager
    {
      public:
        explicit TokenManager(std::vector<Token> tokens);
        bool notAtEnd() const;
        void advanceTokenIterator();
        Token currentToken() const;
        Token nextToken() const;
      private:
        std::vector<Token> m_tokens;
        std::vector<Token>::iterator m_current;
    };
}