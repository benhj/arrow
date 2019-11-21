#pragma once

#include "Evaluator.hpp"
#include "representation/Type.hpp"

namespace jasl {

    class MathExpression;
    class MathExpressionEvaluator : public Evaluator
    {
      public:
        MathExpressionEvaluator(MathExpression expression);
        Type evaluate() const override;
      private:
        MathExpression m_expression;
    };

}