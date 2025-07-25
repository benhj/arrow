/// (c) Ben Jones 2019 - present

#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace arrow {
    
    class OperatorExpression final : public Expression
    {
      public:
        OperatorExpression(long const lineNumber);

        OperatorExpression & withLeft(std::shared_ptr<Expression> expressionLeft);
        OperatorExpression & withRight(std::shared_ptr<Expression> expressionRight);
        OperatorExpression & withOperator(Token op);
        std::shared_ptr<Expression> const & getExpressionLeft() const;
        std::shared_ptr<Expression> const & getExpressionRight() const;
        Token const & getOperator() const;
        std::string toString() const override;

      private:
        std::shared_ptr<Expression> m_expressionLeft;
        Token m_op;
        std::shared_ptr<Expression> m_expressionRight;
    };

}