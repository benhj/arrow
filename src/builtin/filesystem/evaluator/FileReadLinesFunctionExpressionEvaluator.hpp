/// (c) Ben Jones 2019

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/filesystem/FileReadLinesFunctionExpression.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class FileReadLinesFunctionExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        explicit FileReadLinesFunctionExpressionEvaluator(FileReadLinesFunctionExpression expression);
        Type evaluate(Environment&) const override;
      private:
        FileReadLinesFunctionExpression m_expression;
    };
}