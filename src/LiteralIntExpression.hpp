#pragma once

#include "Expression.hpp"
#include "Token.hpp"
#include <memory>
#include <utility>

namespace jasl {
    
    class LiteralIntExpression : public Expression
    {
      public:
        LiteralIntExpression()
         : Expression()
         , m_intToken()
        {
        }

        LiteralIntExpression & withIntToken(Token intToken)
        {
            m_intToken = std::move(intToken);
            return *this;
        }

        Token getIntToken() const
        {
            return m_intToken;
        }

        std::string toString() const override 
        {
            std::string str("Literal int expression: ");
            return str.append(m_intToken.raw);
        }

      private:
        Token m_intToken;
    };

}