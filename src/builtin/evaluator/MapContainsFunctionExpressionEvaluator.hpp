/// (c) Ben Jones 2020 - present

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/MapContainsFunctionExpression.hpp"
#include "representation/Cache.hpp"

namespace arrow {
    class MapContainsFunctionExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        explicit MapContainsFunctionExpressionEvaluator(MapContainsFunctionExpression expression);
        Type evaluate(Cache&) const override;
      private:
        MapContainsFunctionExpression m_expression;
    };
}