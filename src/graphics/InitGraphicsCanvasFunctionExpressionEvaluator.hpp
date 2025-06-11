/// (c) Ben Jones 2025 - present

#pragma once

#include "InitGraphicsCanvasFunctionExpression.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class InitGraphicsCanvasFunctionExpressionEvaluator final : public ExpressionEvaluator
    {
      public:
        explicit InitGraphicsCanvasFunctionExpressionEvaluator(InitGraphicsCanvasFunctionExpression expression);
        Type evaluate(Environment&) const override;
      private:
        InitGraphicsCanvasFunctionExpression m_expression;
    };
}