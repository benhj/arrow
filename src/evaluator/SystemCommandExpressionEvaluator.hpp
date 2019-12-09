/// (c) Ben Jones 2019

#pragma once

#include "ExpressionEvaluator.hpp"
#include "expressions/SystemCommandExpression.hpp"
#include "representation/Cache.hpp"

namespace arrow {
    class SystemCommandExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        explicit SystemCommandExpressionEvaluator(SystemCommandExpression expression);
        Type evaluate(Cache&) const override;
      private:
        SystemCommandExpression m_expression;
    };
}