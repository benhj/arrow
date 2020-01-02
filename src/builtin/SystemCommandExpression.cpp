/// (c) Ben Jones 2019 - present

#include "SystemCommandExpression.hpp"
#include "evaluator/SystemCommandExpressionEvaluator.hpp"

namespace arrow {

    SystemCommandExpression::SystemCommandExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> SystemCommandExpression::getEvaluator() const
    {
        return std::make_shared<SystemCommandExpressionEvaluator>(*this);
    }

    void SystemCommandExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression>
    SystemCommandExpression::getExpression() const
    {
        return m_expression;
    }

    std::string SystemCommandExpression::toString() const
    {
        return ""; // todo
    }
}