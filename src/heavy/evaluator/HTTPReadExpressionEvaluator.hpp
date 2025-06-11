/// (c) Ben Jones 2019

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "heavy/HTTPReadExpression.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class HTTPReadExpressionEvaluator final : public ExpressionEvaluator
    {
      public:
        explicit HTTPReadExpressionEvaluator(HTTPReadExpression expression);
        Type evaluate(Environment&) const override;
      private:
        HTTPReadExpression m_expression;
    };
}