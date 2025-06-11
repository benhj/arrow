/// (c) Ben Jones 2020 - present

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/MapContainsFunctionExpression.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class MapContainsFunctionExpressionEvaluator final : public ExpressionEvaluator
    {
      public:
        explicit MapContainsFunctionExpressionEvaluator(MapContainsFunctionExpression expression);
        Type evaluate(Environment&) const override;
      private:
        MapContainsFunctionExpression m_expression;
    };
}