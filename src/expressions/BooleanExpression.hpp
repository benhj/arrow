/// (c) Ben Jones 2019

#pragma once

#include "OperatorExpression.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace arrow {


    class BooleanExpression final : public Expression
    {
      public:
        BooleanExpression(long const lineNumber);

        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;
        BooleanExpression & withLeft(std::shared_ptr<Expression> expressionLeft);
        BooleanExpression & withRight(std::shared_ptr<Expression> expressionRight);
        BooleanExpression & withOperator(Token op);
        std::shared_ptr<Expression> const & getExpressionLeft() const;
        std::shared_ptr<Expression> const & getExpressionRight() const;
        Token const & getOperator() const;
        std::string toString() const override;

      private:
        std::shared_ptr<OperatorExpression> m_operatorExp;
    };

}