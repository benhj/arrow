#pragma once

#include "Expression.hpp"
#include "Token.hpp"
#include <memory>

namespace jasl {
    
    class OperatorExpression : public Expression
    {
      public:
        OperatorExpression() 
        : Expression()
        , m_expressionLeft{nullptr}
        , m_op()
        , m_expressionRight{nullptr}
        {
        }

        OperatorExpression & withLeft(std::shared_ptr<Expression> expressionLeft)
        {
            m_expressionLeft = std::move(expressionLeft);
            return *this;
        }
        OperatorExpression & withRight(std::shared_ptr<Expression> expressionRight)
        {
            m_expressionRight = std::move(expressionRight);
            return *this;
        }
        OperatorExpression & withOperator(Token op)
        {
            m_op = std::move(op);
            return *this;
        }
        std::shared_ptr<Expression> getExpressionLeft() const
        {
            return m_expressionLeft;
        }
        std::shared_ptr<Expression> getExpressionRight() const
        {
            return m_expressionRight;
        }
        Token getOperator() const
        {
            return m_op;
        }
        std::string toString() const override 
        {
            std::string str("\nLeft: ");
            str.append(m_expressionLeft->toString());
            str.append("\nOperator: ");
            str.append(m_op.raw);
            str.append("\nRight: ");
            str.append(m_expressionRight->toString());
            return str;
        }

      private:
        std::shared_ptr<Expression> m_expressionLeft;
        Token m_op;
        std::shared_ptr<Expression> m_expressionRight;
    };

}