/// (c) Ben Jones 2019

#pragma once

#include "ExpressionEvaluator.hpp"
#include "expressions/MatchesExpression.hpp"
#include "representation/Type.hpp"
#include "representation/Environment.hpp"
#include <memory>

namespace arrow {

    class MatchesExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        explicit MatchesExpressionEvaluator(MatchesExpression expression);
        Type evaluate(Environment&) const override;
      private:
        MatchesExpression m_expression;
    };
}