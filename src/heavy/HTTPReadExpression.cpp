/// (c) Ben Jones 2020 - present

#include "HTTPReadExpression.hpp"
#include "evaluator/HTTPReadExpressionEvaluator.hpp"

namespace arrow {

    HTTPReadExpression::HTTPReadExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> HTTPReadExpression::getEvaluator() const
    {
        return std::make_shared<HTTPReadExpressionEvaluator>(*this);
    }

    void HTTPReadExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression>
    HTTPReadExpression::getExpression() const
    {
        return m_expression;
    }

    std::string HTTPReadExpression::toString() const
    {
        return ""; // todo
    }
}