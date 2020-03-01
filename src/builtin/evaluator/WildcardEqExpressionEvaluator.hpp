/// (c) Ben Jones 2020 - present

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/WildcardEqExpression.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class WildcardEqExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        explicit WildcardEqExpressionEvaluator(WildcardEqExpression expression);
        Type evaluate(Environment&) const override;
      private:
        WildcardEqExpression m_expression;
    };
}