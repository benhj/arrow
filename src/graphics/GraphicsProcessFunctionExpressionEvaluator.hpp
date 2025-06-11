/// (c) Ben Jones 2025 - present

#pragma once

#include "GraphicsProcessFunctionExpression.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class GraphicsProcessFunctionExpressionEvaluator final : public ExpressionEvaluator
    {
      public:
        explicit GraphicsProcessFunctionExpressionEvaluator(GraphicsProcessFunctionExpression expression);
        Type evaluate(Environment&) const override;
      private:
        GraphicsProcessFunctionExpression m_expression;
    };
}