/// (c) Ben Jones 2019

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/filesystem/FolderListFunctionExpression.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class FolderListFunctionExpressionEvaluator final : public ExpressionEvaluator
    {
      public:
        explicit FolderListFunctionExpressionEvaluator(FolderListFunctionExpression expression);
        Type evaluate(Environment&) const override;
      private:
        FolderListFunctionExpression m_expression;
    };
}