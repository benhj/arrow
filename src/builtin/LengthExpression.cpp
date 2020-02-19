/// (c) Ben Jones 2020 - present

#include "LengthExpression.hpp"
#include "evaluator/LengthExpressionEvaluator.hpp"

namespace arrow {

    LengthExpression::LengthExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> LengthExpression::getEvaluator() const
    {
        return std::make_shared<LengthExpressionEvaluator>(*this);
    }

    void LengthExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression>
    LengthExpression::getExpression() const
    {
        return m_expression;
    }

    std::string LengthExpression::toString() const
    {
        return ""; // todo
    }
}