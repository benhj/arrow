/// (c) Ben Jones 2019 - present

#pragma once

#include "expressions/builtin/BuiltInFunctionExpression.hpp"
#include "expressions/builtin/BuiltInFunctionExpressionBuilder.hpp"
#include "expressions/Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>
#include <vector>

namespace arrow {
    
    class FileReadBytesFunctionExpression : public Expression, public BuiltInFunctionExpression
    {
      public:
        FileReadBytesFunctionExpression(long const lineNumber);
        void setExpression(std::shared_ptr<Expression> expression) override;
        std::shared_ptr<Expression> getExpression() const;
        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;
        std::string toString() const override;
      private:
        std::shared_ptr<Expression> m_expression;
    };

    class FileReadBytesFunctionExpressionBuilder : public BuiltInFunctionExpressionBuilder
    {
      public:
        std::string getName() const override {
            return "file_read_bytes";
        }
        std::shared_ptr<Expression> build(long const ln) const override {
            return std::make_shared<FileReadBytesFunctionExpression>(ln);
        }
    };
}