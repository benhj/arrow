/// (c) Ben Jones 2019

#pragma once

#include "ExpressionEvaluator.hpp"
#include "expressions/MathExpression.hpp"
#include "representation/Type.hpp"

namespace arrow {

    class MathExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        MathExpressionEvaluator(MathExpression expression);
        Type evaluate(Environment & cache) const override;
      private:
        MathExpression m_expression;
    };

}