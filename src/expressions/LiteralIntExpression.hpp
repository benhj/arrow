#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
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

        Type evaluate() const override
        {
            return {TypeDescriptor::Int, {std::stoll(m_intToken.raw)}};
        }

        DecayType decayType() const override
        {
            return DecayType::DECAYS_TO_INT;
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
            std::string str("\nLiteral int expression: ");
            return str.append(m_intToken.raw);
        }

      private:
        Token m_intToken;
    };

}