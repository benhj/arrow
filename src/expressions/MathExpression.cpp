/// (c) Ben Jones 2019 - present

#include "MathExpression.hpp"
#include "evaluator/MathExpressionEvaluator.hpp"

namespace arrow {

    MathExpression::MathExpression(long const lineNumber) 
      : Expression(lineNumber)
      , m_operatorExp(std::make_shared<OperatorExpression>(lineNumber))
    {
    }

    std::shared_ptr<ExpressionEvaluator> MathExpression::getEvaluator() const
    {
        return std::make_shared<MathExpressionEvaluator>(*this);
    }

    MathExpression & MathExpression::withLeft(std::shared_ptr<Expression> expressionLeft)
    {
        m_operatorExp->withLeft(std::move(expressionLeft));
        return *this;
    }
    MathExpression & MathExpression::withRight(std::shared_ptr<Expression> expressionRight)
    {
        m_operatorExp->withRight(std::move(expressionRight));
        return *this;
    }
    MathExpression & MathExpression::withOperator(Token op)
    {
        m_operatorExp->withOperator(std::move(op));
        return *this;
    }
    std::shared_ptr<Expression> MathExpression::getExpressionLeft() const
    {
        return m_operatorExp->getExpressionLeft();
    }
    std::shared_ptr<Expression> MathExpression::getExpressionRight() const
    {
        return m_operatorExp->getExpressionRight();
    }
    Token MathExpression::getOperator() const
    {
        return m_operatorExp->getOperator();
    }
    std::string MathExpression::toString() const
    {
        return m_operatorExp->toString();
    }
}