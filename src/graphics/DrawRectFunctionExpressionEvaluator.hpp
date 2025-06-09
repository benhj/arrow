/// (c) Ben Jones 2025 - present

#pragma once

#include "DrawRectFunctionExpression.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class DrawRectFunctionExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        explicit DrawRectFunctionExpressionEvaluator(DrawRectFunctionExpression expression);
        Type evaluate(Environment&) const override;
      private:
        DrawRectFunctionExpression m_expression;
    };
} // namespace arrow