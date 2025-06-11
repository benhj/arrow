/// (c) Ben Jones 2025 - present

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/ExpFunctionExpression.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class ExpFunctionExpressionEvaluator final : public ExpressionEvaluator
    {
      public:
        explicit ExpFunctionExpressionEvaluator(ExpFunctionExpression expression);
        Type evaluate(Environment&) const override;
      private:
        ExpFunctionExpression m_expression;
    };
}