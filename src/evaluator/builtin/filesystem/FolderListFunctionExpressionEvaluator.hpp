/// (c) Ben Jones 2019

#pragma once

#include "evaluator/ExpressionEvaluator.hpp"
#include "expressions/builtin/filesystem/FolderListFunctionExpression.hpp"
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