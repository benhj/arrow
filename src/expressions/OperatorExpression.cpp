#include "OperatorExpression.hpp"

namespace jasl {

    OperatorExpression::OperatorExpression() 
    : Expression()
    , m_expressionLeft{nullptr}
    , m_op()
    , m_expressionRight{nullptr}
    {
    }

    std::shared_ptr<Evaluator> OperatorExpression::getEvaluator() const
    {
        return nullptr;
    }

    Type OperatorExpression::evaluate() const
    {
        return {TypeDescriptor::None, {false}};
    }

    DecayType OperatorExpression::decayType() const
    {
        return DecayType::DECAYS_TO_NONE;
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
    std::shared_ptr<Expression> OperatorExpression::getExpressionLeft() const
    {
        return m_expressionLeft;
    }
    std::shared_ptr<Expression> OperatorExpression::getExpressionRight() const
    {
        return m_expressionRight;
    }
    Token OperatorExpression::getOperator() const
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