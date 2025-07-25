/// (c) Ben Jones 2019 - present

#pragma once

#include "OperatorExpression.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace arrow {
    
    class MathExpression final : public Expression
    {
      public:
        MathExpression(long const lineNumber);

        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;

        MathExpression & withLeft(std::shared_ptr<Expression> expressionLeft);
        MathExpression & withRight(std::shared_ptr<Expression> expressionRight);
        MathExpression & withOperator(Token op);
        std::shared_ptr<Expression> const & getExpressionLeft() const;
        std::shared_ptr<Expression> const & getExpressionRight() const;
        Token const & getOperator() const;
        std::string toString() const override;

      private:
        std::shared_ptr<OperatorExpression> m_operatorExp;
    };

}