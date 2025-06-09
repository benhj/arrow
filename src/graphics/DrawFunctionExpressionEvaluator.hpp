/// (c) Ben Jones 2025 - present

#pragma once

#include "DrawFunctionExpression.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class DrawFunctionExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        explicit DrawFunctionExpressionEvaluator(DrawFunctionExpression expression);
        Type evaluate(Environment&) const override;
      private:
        DrawFunctionExpression m_expression;
    };
}