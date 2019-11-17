#pragma once

#include "Expression.hpp"
#include "Token.hpp"
#include <memory>
#include <utility>

namespace jasl {
    
    class LiteralStringExpression : public Expression
    {
      public:
        LiteralStringExpression()
         : Expression()
         , m_stringToken()
        {
        }

        LiteralStringExpression & withStringToken(Token stringToken)
        {
            m_stringToken = std::move(stringToken);
            return *this;
        }

        Token getStringToken() const
        {
            return m_stringToken;
        }

        std::string toString() const override 
        {
            return m_stringToken.raw;
        }

      private:
        Token m_stringToken;
    };

}