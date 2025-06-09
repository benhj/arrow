/// (c) Ben Jones 2025 - present

#pragma once

#include "FillCircleFunctionExpression.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class FillCircleFunctionExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        explicit FillCircleFunctionExpressionEvaluator(FillCircleFunctionExpression expression);
        Type evaluate(Environment&) const override;
      private:
        FillCircleFunctionExpression m_expression;
    };
} // namespace arrow