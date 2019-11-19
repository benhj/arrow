#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
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

        Type evaluate() const override
        {
            return {TypeDescriptor::String, m_stringToken.raw};
        }

        DecayType decayType() const override
        {
            return DecayType::DECAYS_TO_STRING;
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
            std::string str("\nLiteral string expression: ");
            return str.append(m_stringToken.raw);
        }

      private:
        Token m_stringToken;
    };

}