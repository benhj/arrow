/// (c) Ben Jones 2025 - present

#include "GetKeyPressExpression.hpp"
#include "evaluator/GetKeyPressExpressionEvaluator.hpp"

namespace arrow {

    GetKeyPressExpression::GetKeyPressExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> GetKeyPressExpression::getEvaluator() const
    {
        return std::make_shared<GetKeyPressExpressionEvaluator>(*this);
    }

    void GetKeyPressExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression> const &
    GetKeyPressExpression::getExpression() const
    {
        return m_expression;
    }

    std::string GetKeyPressExpression::toString() const
    {
        return ""; // todo
    }
}