/// (c) Ben Jones 2025 - present

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/ToIntFunctionExpression.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class ToIntFunctionExpressionEvaluator final : public ExpressionEvaluator
    {
      public:
        explicit ToIntFunctionExpressionEvaluator(ToIntFunctionExpression expression);
        Type evaluate(Environment&) const override;
      private:
        ToIntFunctionExpression m_expression;
    };
}