/// (c) Ben Jones 2019

#pragma once

#include "ExpressionEvaluator.hpp"
#include "expressions/RandomFunctionExpression.hpp"
#include "representation/Cache.hpp"
#include <random>

namespace arrow {
    class RandomFunctionExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        explicit RandomFunctionExpressionEvaluator(RandomFunctionExpression expression);
        Type evaluate(Cache&) const override;
      private:
        RandomFunctionExpression m_expression;
        std::random_device mutable m_r;
    };
}