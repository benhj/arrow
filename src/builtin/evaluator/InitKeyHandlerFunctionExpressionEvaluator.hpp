/// (c) Ben Jones 2025 - present

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/InitKeyHandlerFunctionExpression.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class InitKeyHandlerFunctionExpressionEvaluator final : public ExpressionEvaluator
    {
      public:
        explicit InitKeyHandlerFunctionExpressionEvaluator(InitKeyHandlerFunctionExpression expression);
        Type evaluate(Environment&) const override;
      private:
        InitKeyHandlerFunctionExpression m_expression;
    };
}