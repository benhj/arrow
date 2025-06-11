/// (c) Ben Jones 2019

#pragma once

#include "ExpressionEvaluator.hpp"
#include "expressions/FunctionExpression.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class FunctionExpressionEvaluator final : public ExpressionEvaluator
    {
      public:
        explicit FunctionExpressionEvaluator(FunctionExpression expression);
        Type evaluate(Environment&) const override;
      private:
        FunctionExpression m_expression;
    };
}