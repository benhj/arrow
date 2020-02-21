/// (c) Ben Jones 2020 - present

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/MapKeysFunctionExpression.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class MapKeysFunctionExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        explicit MapKeysFunctionExpressionEvaluator(MapKeysFunctionExpression expression);
        Type evaluate(Environment&) const override;
      private:
        MapKeysFunctionExpression m_expression;
    };
}