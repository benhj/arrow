#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace arrow {
    
    class LiteralIntExpression : public Expression
    {
      public:
        LiteralIntExpression(long const lineNumber);

        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;

        DecayType decayType() const override;

        LiteralIntExpression & withIntToken(Token intToken);

        Token getIntToken() const;

        std::string toString() const override;

      private:
        Token m_intToken;
    };

}