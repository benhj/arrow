/// (c) Ben Jones 2019

#pragma once

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "builtin/filesystem/FileReadBytesFunctionExpression.hpp"
#include "representation/Cache.hpp"

namespace arrow {
    class FileReadBytesFunctionExpressionEvaluator : public ExpressionEvaluator
    {
      public:
        explicit FileReadBytesFunctionExpressionEvaluator(FileReadBytesFunctionExpression expression);
        Type evaluate(Cache&) const override;
      private:
        FileReadBytesFunctionExpression m_expression;
    };
}