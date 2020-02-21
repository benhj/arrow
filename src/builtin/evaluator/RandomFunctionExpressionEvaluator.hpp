/// (c) Ben Jones 2019

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/RandomFunctionExpression.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class RandomFunctionExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        explicit RandomFunctionExpressionEvaluator(RandomFunctionExpression expression);
        Type evaluate(Environment&) const override;
      private:
        RandomFunctionExpression m_expression;
    };
}