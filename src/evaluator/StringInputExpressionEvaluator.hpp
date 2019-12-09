/// (c) Ben Jones 2019

#pragma once

#include "ExpressionEvaluator.hpp"
#include "expressions/StringInputExpression.hpp"
#include "representation/Cache.hpp"

namespace arrow {
    class StringInputExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        explicit StringInputExpressionEvaluator(StringInputExpression expression);
        Type evaluate(Cache&) const override;
      private:
        StringInputExpression m_expression;
    };
}