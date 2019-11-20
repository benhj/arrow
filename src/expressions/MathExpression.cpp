#include "MathExpression.hpp"
#include "evaluator/MathExpressionEvaluator.hpp"

namespace jasl {

    MathExpression::MathExpression() 
    : Expression()
    , m_operatorExp(std::make_shared<OperatorExpression>())
    {
    }

    std::shared_ptr<Evaluator> MathExpression::getEvaluator() const
    {
        return std::make_shared<MathExpressionEvaluator>(*this);
    }

    DecayType MathExpression::decayType() const
    {
        return DecayType::DECAYS_TO_REAL;
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