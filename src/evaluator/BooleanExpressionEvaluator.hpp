#pragma once

#include "Evaluator.hpp"
#include "representation/Type.hpp"

namespace jasl {

    class BooleanExpression;
    class BooleanExpressionEvaluator : public Evaluator
    {
      public:
        BooleanExpressionEvaluator(BooleanExpression expression);
        Type evaluate() const override;
      private:
        BooleanExpression m_expression;
    };

}