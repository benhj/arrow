/// (c) Ben Jones 2019

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/SinFunctionExpression.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class SinFunctionExpressionEvaluator final : public ExpressionEvaluator
    {
      public:
        explicit SinFunctionExpressionEvaluator(SinFunctionExpression expression);
        Type evaluate(Environment&) const override;
      private:
        SinFunctionExpression m_expression;
    };
}