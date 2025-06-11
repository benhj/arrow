/// (c) Ben Jones 2020 - present

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/ArgExpression.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class ArgExpressionEvaluator final : public ExpressionEvaluator
    {
      public:
        explicit ArgExpressionEvaluator(ArgExpression expression);
        Type evaluate(Environment&) const override;
      private:
        ArgExpression m_expression;
    };
}