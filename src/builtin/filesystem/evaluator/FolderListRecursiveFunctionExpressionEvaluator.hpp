/// (c) Ben Jones 2019

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/filesystem/FolderListRecursiveFunctionExpression.hpp"
#include "representation/Cache.hpp"

namespace arrow {
    class FolderListRecursiveFunctionExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        explicit FolderListRecursiveFunctionExpressionEvaluator(FolderListRecursiveFunctionExpression expression);
        Type evaluate(Cache&) const override;
      private:
        FolderListRecursiveFunctionExpression m_expression;
    };
}