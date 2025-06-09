/// (c) Ben Jones 2025 - present

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/CosFunctionExpression.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class CosFunctionExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        explicit CosFunctionExpressionEvaluator(CosFunctionExpression expression);
        Type evaluate(Environment&) const override;
      private:
        CosFunctionExpression m_expression;
    };
}