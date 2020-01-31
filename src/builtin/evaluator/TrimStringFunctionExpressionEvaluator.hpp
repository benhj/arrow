/// (c) Ben Jones 2019

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/TrimStringFunctionExpression.hpp"
#include "representation/Cache.hpp"

namespace arrow {
    class TrimStringFunctionExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        explicit TrimStringFunctionExpressionEvaluator(TrimStringFunctionExpression expression);
        Type evaluate(Cache&) const override;
      private:
        TrimStringFunctionExpression m_expression;
    };
}