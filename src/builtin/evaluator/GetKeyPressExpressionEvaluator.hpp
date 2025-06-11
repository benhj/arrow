/// (c) Ben Jones 2025 - present

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/GetKeyPressExpression.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class GetKeyPressExpressionEvaluator final : public ExpressionEvaluator
    {
      public:
        explicit GetKeyPressExpressionEvaluator(GetKeyPressExpression expression);
        Type evaluate(Environment&) const override;
      private:
        GetKeyPressExpression m_expression;
    };
}