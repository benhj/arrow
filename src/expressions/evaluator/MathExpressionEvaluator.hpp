/// (c) Ben Jones 2019

#pragma once

#include "ExpressionEvaluator.hpp"
#include "expressions/MathExpression.hpp"
#include "representation/Type.hpp"

namespace arrow {

    class MathExpressionEvaluator final : public ExpressionEvaluator
    {
      public:
        MathExpressionEvaluator(MathExpression expression);
        Type evaluate(Environment & environment) const override;
      private:
        MathExpression m_expression;
    };

}