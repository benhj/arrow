/// (c) Ben Jones 2020

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/TokenizeFunctionExpression.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class TokenizeFunctionExpressionEvaluator final : public ExpressionEvaluator
    {
      public:
        explicit TokenizeFunctionExpressionEvaluator(TokenizeFunctionExpression expression);
        Type evaluate(Environment&) const override;
      private:
        TokenizeFunctionExpression m_expression;
    };
}