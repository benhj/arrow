/// (c) Ben Jones 2019 - present

#include "OperatorExpression.hpp"

namespace arrow {

    OperatorExpression::OperatorExpression(long const lineNumber) 
      : Expression(lineNumber)
      , m_expressionLeft{nullptr}
      , m_op()
      , m_expressionRight{nullptr}
    {
    }

    OperatorExpression & OperatorExpression::withLeft(std::shared_ptr<Expression> expressionLeft)
    {
        m_expressionLeft = std::move(expressionLeft);
        return *this;
    }
    OperatorExpression & OperatorExpression::withRight(std::shared_ptr<Expression> expressionRight)
    {
        m_expressionRight = std::move(expressionRight);
        return *this;
    }
    OperatorExpression & OperatorExpression::withOperator(Token op)
    {
        m_op = std::move(op);
        return *this;
    }
    std::shared_ptr<Expression> const & OperatorExpression::getExpressionLeft() const
    {
        return m_expressionLeft;
    }
    std::shared_ptr<Expression> const & OperatorExpression::getExpressionRight() const
    {
        return m_expressionRight;
    }
    Token const & OperatorExpression::getOperator() const
    {
        return m_op;
    }
    std::string OperatorExpression::toString() const
    {
        std::string str("\nBegin expression: ");
        str.append("\nLeft: ");
        str.append(m_expressionLeft->toString());
        str.append("\nOperator: ");
        str.append(m_op.raw);
        str.append("\nRight: ");
        str.append(m_expressionRight->toString());
        str.append("\nEnd expression");
        return str;
    }
}