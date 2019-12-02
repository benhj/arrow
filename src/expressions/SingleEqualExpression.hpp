#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>


namespace arrow {
    class SingleEqualExpression : public Expression
    {
      public:
        SingleEqualExpression(long const lineNumber);

        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;

        DecayType decayType() const override;

        SingleEqualExpression & withToken(Token token);

        std::string toString() const override;

      private:
        Token m_token;
    };
}