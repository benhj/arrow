/// (c) Ben Jones 2019

#pragma once

#include "evaluator/ExpressionEvaluator.hpp"
#include "expressions/builtin/SqrtFunctionExpression.hpp"
#include "representation/Cache.hpp"

namespace arrow {
    class SqrtFunctionExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        explicit SqrtFunctionExpressionEvaluator(SqrtFunctionExpression expression);
        Type evaluate(Cache&) const override;
      private:
        SqrtFunctionExpression m_expression;
    };
}