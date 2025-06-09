/// (c) Ben Jones 2025 - present

#pragma once

#include "DrawCircleFunctionExpression.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class DrawCircleFunctionExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        explicit DrawCircleFunctionExpressionEvaluator(DrawCircleFunctionExpression expression);
        Type evaluate(Environment&) const override;
      private:
        DrawCircleFunctionExpression m_expression;
    };
} // namespace arrow