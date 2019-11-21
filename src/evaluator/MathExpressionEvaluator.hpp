#pragma once

#include "ExpressionEvaluator.hpp"
#include "expressions/MathExpression.hpp"
#include "representation/Type.hpp"

namespace jasl {

    class MathExpression;
    class MathExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        MathExpressionEvaluator(MathExpression expression);
        Type evaluate(Cache & cache) const override;
      private:
        MathExpression m_expression;
    };

}