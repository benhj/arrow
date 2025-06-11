/// (c) Ben Jones 2019

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/RegexFunctionExpression.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class RegexFunctionExpressionEvaluator final : public ExpressionEvaluator
    {
      public:
        explicit RegexFunctionExpressionEvaluator(RegexFunctionExpression expression);
        Type evaluate(Environment&) const override;
      private:
        RegexFunctionExpression m_expression;
    };
}