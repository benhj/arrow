/// (c) Ben Jones 2019

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/filesystem/FileWriteBytesFunctionExpression.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class FileWriteBytesFunctionExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        explicit FileWriteBytesFunctionExpressionEvaluator(FileWriteBytesFunctionExpression expression);
        Type evaluate(Environment&) const override;
      private:
        FileWriteBytesFunctionExpression m_expression;
    };
}