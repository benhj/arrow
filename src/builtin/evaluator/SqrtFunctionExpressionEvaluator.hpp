/// (c) Ben Jones 2019

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/SqrtFunctionExpression.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class SqrtFunctionExpressionEvaluator final : public ExpressionEvaluator
    {
      public:
        explicit SqrtFunctionExpressionEvaluator(SqrtFunctionExpression expression);
        Type evaluate(Environment&) const override;
      private:
        SqrtFunctionExpression m_expression;
    };
}