#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace jasl {
    
    class IdentifierExpression : public Expression
    {
      public:
        IdentifierExpression();

        Type evaluate() const override;

        DecayType decayType() const override;

        IdentifierExpression & withIntToken(Token identifier);

        Token getIntToken() const;

        std::string toString() const override;

      private:
        Token m_identifier;
    };

}