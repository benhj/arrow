/// (c) Ben Jones 2025 - present

#pragma once

#include "FillRectFunctionExpression.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class FillRectFunctionExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        explicit FillRectFunctionExpressionEvaluator(FillRectFunctionExpression expression);
        Type evaluate(Environment&) const override;
      private:
        FillRectFunctionExpression m_expression;
    };
} // namespace arrow