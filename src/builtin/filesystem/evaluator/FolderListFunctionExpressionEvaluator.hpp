/// (c) Ben Jones 2019

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/filesystem/FolderListFunctionExpression.hpp"
#include "representation/Cache.hpp"

namespace arrow {
    class FolderListFunctionExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        explicit FolderListFunctionExpressionEvaluator(FolderListFunctionExpression expression);
        Type evaluate(Cache&) const override;
      private:
        FolderListFunctionExpression m_expression;
    };
}