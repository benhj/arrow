/// (c) Ben Jones 2020 - present

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/LengthExpression.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class LengthExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        explicit LengthExpressionEvaluator(LengthExpression expression);
        Type evaluate(Environment&) const override;
      private:
        LengthExpression m_expression;
    };
}