/// (c) Ben Jones 2020 - present

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/LengthExpression.hpp"
#include "representation/Cache.hpp"

namespace arrow {
    class LengthExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        explicit LengthExpressionEvaluator(LengthExpression expression);
        Type evaluate(Cache&) const override;
      private:
        LengthExpression m_expression;
    };
}