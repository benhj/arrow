/// (c) Ben Jones 2019

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/filesystem/FileReadLinesFunctionExpression.hpp"
#include "representation/Cache.hpp"

namespace arrow {
    class FileReadLinesFunctionExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        explicit FileReadLinesFunctionExpressionEvaluator(FileReadLinesFunctionExpression expression);
        Type evaluate(Cache&) const override;
      private:
        FileReadLinesFunctionExpression m_expression;
    };
}