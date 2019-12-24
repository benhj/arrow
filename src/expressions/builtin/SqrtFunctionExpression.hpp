/// (c) Ben Jones 2019 - present

#pragma once

#include "BuiltInFunctionExpressionBuilder.hpp"
#include "expressions/Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>
#include <vector>

namespace arrow {
    
    class SqrtFunctionExpression : public Expression
    {
      public:
        SqrtFunctionExpression(long const lineNumber);
        SqrtFunctionExpression & 
        withExpression(std::shared_ptr<Expression> expression);
        std::shared_ptr<Expression> getExpression() const;
        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;
        std::string toString() const override;
      private:
        std::shared_ptr<Expression> m_expression;
    };

    class SqrtFunctionExpressionBuilder : public BuiltInFunctionExpressionBuilder
    {
      public:
        std::string getName() const override {
            return "sqrt";
        }
        std::shared_ptr<Expression> build(long const ln) const override {
            return std::make_shared<SqrtFunctionExpression>(ln);
        }
    };

}