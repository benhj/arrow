/// (c) Ben Jones 2019

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/StringInputExpression.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class StringInputExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        explicit StringInputExpressionEvaluator(StringInputExpression expression);
        Type evaluate(Environment&) const override;
      private:
        StringInputExpression m_expression;
    };
}