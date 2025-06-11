/// (c) Ben Jones 2025 - present

#pragma once

#include "DrawLineFunctionExpression.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class DrawLineFunctionExpressionEvaluator final : public ExpressionEvaluator
    {
      public:
        explicit DrawLineFunctionExpressionEvaluator(DrawLineFunctionExpression expression);
        Type evaluate(Environment&) const override;
      private:
        DrawLineFunctionExpression m_expression;
    };
}