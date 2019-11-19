#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>
#include <utility>

namespace jasl {
    
    class IdentifierExpression : public Expression
    {
      public:
        IdentifierExpression()
         : Expression()
         , m_identifier()
        {
        }

        IdentifierExpression & withIntToken(Token identifier)
        {
            m_identifier = std::move(identifier);
            return *this;
        }

        Token getIntToken() const
        {
            return  m_identifier;
        }

        std::string toString() const override 
        {
            std::string str("\nIdentifier expression: ");
            return str.append(m_identifier.raw);
        }

      private:
        Token m_identifier;
    };

}