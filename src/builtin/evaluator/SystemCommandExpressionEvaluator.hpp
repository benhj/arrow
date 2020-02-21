/// (c) Ben Jones 2019

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/SystemCommandExpression.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class SystemCommandExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        explicit SystemCommandExpressionEvaluator(SystemCommandExpression expression);
        Type evaluate(Environment&) const override;
      private:
        SystemCommandExpression m_expression;
    };
}