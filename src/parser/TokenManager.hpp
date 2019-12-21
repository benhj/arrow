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
        bool tokenPlusOneNotAtEnd() const;
        void advanceTokenIterator();
        Token currentToken() const;
        Token nextToken() const;
        std::vector<Token>::iterator retrieveIt() const;
        void revert(std::vector<Token>::iterator orig);
      private:
        std::vector<Token> m_tokens;
        std::vector<Token>::iterator m_current;
    };
}