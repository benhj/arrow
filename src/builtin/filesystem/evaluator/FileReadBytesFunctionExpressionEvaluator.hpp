/// (c) Ben Jones 2019

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/filesystem/FileReadBytesFunctionExpression.hpp"
#include "representation/Environment.hpp"

namespace arrow {
    class FileReadBytesFunctionExpressionEvaluator final : public ExpressionEvaluator
    {
      public:
        explicit FileReadBytesFunctionExpressionEvaluator(FileReadBytesFunctionExpression expression);
        Type evaluate(Environment&) const override;
      private:
        FileReadBytesFunctionExpression m_expression;
    };
}