/// (c) Ben Jones 2025 - present

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/Atan2FunctionExpression.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class Atan2FunctionExpressionEvaluator final : public ExpressionEvaluator
    {
      public:
        explicit Atan2FunctionExpressionEvaluator(Atan2FunctionExpression expression);
        Type evaluate(Environment&) const override;
      private:
        Atan2FunctionExpression m_expression;
    };
}