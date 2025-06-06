/// (c) Ben Jones 2020 - present

#include "InitKeyHandlerFunctionExpression.hpp"
#include "evaluator/InitKeyHandlerFunctionExpressionEvaluator.hpp"

namespace arrow {

    InitKeyHandlerFunctionExpression::InitKeyHandlerFunctionExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> InitKeyHandlerFunctionExpression::getEvaluator() const
    {
        return std::make_shared<InitKeyHandlerFunctionExpressionEvaluator>(*this);
    }

    void InitKeyHandlerFunctionExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression> const &
    InitKeyHandlerFunctionExpression::getExpression() const
    {
        return m_expression;
    }

    std::string InitKeyHandlerFunctionExpression::toString() const
    {
        return ""; // todo
    }
}