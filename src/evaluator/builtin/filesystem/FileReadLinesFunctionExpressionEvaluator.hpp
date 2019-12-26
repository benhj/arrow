/// (c) Ben Jones 2019

#pragma once

#include "evaluator/ExpressionEvaluator.hpp"
#include "expressions/builtin/filesystem/FileReadLinesFunctionExpression.hpp"
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