#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace jasl {
    
    class LiteralIntExpression : public Expression
    {
      public:
        LiteralIntExpression();

        Type evaluate() const override;

        DecayType decayType() const override;

        LiteralIntExpression & withIntToken(Token intToken);

        Token getIntToken() const;

        std::string toString() const override;

      private:
        Token m_intToken;
    };

}