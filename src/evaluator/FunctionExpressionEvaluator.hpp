#pragma once

#include "ExpressionEvaluator.hpp"
#include "expressions/FunctionExpression.hpp"
#include "representation/Cache.hpp"

namespace arrow {
    class FunctionExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        explicit FunctionExpressionEvaluator(FunctionExpression expression);
        Type evaluate(Cache&) const override;
      private:
        FunctionExpression m_expression;
    };
}