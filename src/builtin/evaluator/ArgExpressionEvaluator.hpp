/// (c) Ben Jones 2020 - present

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/ArgExpression.hpp"
#include "representation/Cache.hpp"

namespace arrow {
    class ArgExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        explicit ArgExpressionEvaluator(ArgExpression expression);
        Type evaluate(Cache&) const override;
      private:
        ArgExpression m_expression;
    };
}