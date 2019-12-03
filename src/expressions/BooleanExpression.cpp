#include "BooleanExpression.hpp"
#include "evaluator/BooleanExpressionEvaluator.hpp"

namespace arrow {

    BooleanExpression::BooleanExpression(long const lineNumber) 
      : Expression(lineNumber)
      , m_operatorExp(std::make_shared<OperatorExpression>(lineNumber))
    {
    }

    std::shared_ptr<ExpressionEvaluator> BooleanExpression::getEvaluator() const
    {
        return std::make_shared<BooleanExpressionEvaluator>(*this);
    }

    BooleanExpression & 
    BooleanExpression::withLeft(std::shared_ptr<Expression> expressionLeft)
    {
        m_operatorExp->withLeft(std::move(expressionLeft));
        return *this;
    }
    BooleanExpression & 
    BooleanExpression::withRight(std::shared_ptr<Expression> expressionRight)
    {
        m_operatorExp->withRight(std::move(expressionRight));
        return *this;
    }
    BooleanExpression & BooleanExpression::withOperator(Token op)
    {
        m_operatorExp->withOperator(std::move(op));
        return *this;
    }
    std::shared_ptr<Expression> BooleanExpression::getExpressionLeft() const
    {
        return m_operatorExp->getExpressionLeft();
    }
    std::shared_ptr<Expression> BooleanExpression::getExpressionRight() const
    {
        return m_operatorExp->getExpressionRight();
    }
    Token BooleanExpression::getOperator() const
    {
        return m_operatorExp->getOperator();
    }
    std::string BooleanExpression::toString() const
    {
        return m_operatorExp->toString();
    }

}