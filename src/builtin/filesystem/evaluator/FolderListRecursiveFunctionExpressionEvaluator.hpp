/// (c) Ben Jones 2019

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/filesystem/FolderListRecursiveFunctionExpression.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class FolderListRecursiveFunctionExpressionEvaluator final : public ExpressionEvaluator
    {
      public:
        explicit FolderListRecursiveFunctionExpressionEvaluator(FolderListRecursiveFunctionExpression expression);
        Type evaluate(Environment&) const override;
      private:
        FolderListRecursiveFunctionExpression m_expression;
    };
}