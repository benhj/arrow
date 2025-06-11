/// (c) Ben Jones 2020 - present

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/TypeExpression.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class TypeExpressionEvaluator final : public ExpressionEvaluator
    {
      public:
        explicit TypeExpressionEvaluator(TypeExpression expression);
        Type evaluate(Environment&) const override;
      private:
        TypeExpression m_expression;
    };
}