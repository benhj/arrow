/// (c) Ben Jones 2020 - present

#include "ArgExpression.hpp"
#include "evaluator/ArgExpressionEvaluator.hpp"

namespace arrow {

    ArgExpression::ArgExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> ArgExpression::getEvaluator() const
    {
        return std::make_shared<ArgExpressionEvaluator>(*this);
    }

    void ArgExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression> const &
    ArgExpression::getExpression() const
    {
        return m_expression;
    }

    std::string ArgExpression::toString() const
    {
        return ""; // todo
    }
}