/// (c) Ben Jones 2020 - present

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/TypeExpression.hpp"
#include "representation/Cache.hpp"

namespace arrow {
    class TypeExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        explicit TypeExpressionEvaluator(TypeExpression expression);
        Type evaluate(Cache&) const override;
      private:
        TypeExpression m_expression;
    };
}