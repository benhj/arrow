#pragma once

#include "Expression.hpp"
#include "Token.hpp"
#include <memory>
#include <utility>

namespace jasl {
    
    class LiteralRealExpression : public Expression
    {
      public:
        LiteralRealExpression()
         : Expression()
         , m_realToken()
        {
        }

        LiteralRealExpression & withRealToken(Token realToken)
        {
            m_realToken = std::move(realToken);
            return *this;
        }

        Token getRealToken() const
        {
            return m_realToken;
        }

        std::string toString() const override 
        {
            std::string str("Literal real expression: ");
            return str.append(m_realToken.raw);
        }

      private:
        Token m_realToken;
    };

}