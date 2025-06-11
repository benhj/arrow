/// (c) Ben Jones 2019

#pragma once

#include "ExpressionEvaluator.hpp"
#include "expressions/BooleanExpression.hpp"
#include "representation/Type.hpp"

namespace arrow {

    class BooleanExpressionEvaluator final : public ExpressionEvaluator
    {
      public:
        BooleanExpressionEvaluator(BooleanExpression expression);
        Type evaluate(Environment&) const override;
      private:
        BooleanExpression m_expression;
    };

}