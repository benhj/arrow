#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
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

        Type evaluate() const override
        {
            return {TypeDescriptor::Real, {false}};
        }

        DecayType decayType() const override
        {
            return DecayType::DECAYS_TO_REAL;
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
            std::string str("\nLiteral real expression: ");
            return str.append(m_realToken.raw);
        }

      private:
        Token m_realToken;
    };

}