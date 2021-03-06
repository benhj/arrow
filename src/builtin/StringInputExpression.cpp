/// (c) Ben Jones 2019 - present

#include "StringInputExpression.hpp"
#include "evaluator/StringInputExpressionEvaluator.hpp"

namespace arrow {

    StringInputExpression::StringInputExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> StringInputExpression::getEvaluator() const
    {
        return std::make_shared<StringInputExpressionEvaluator>(*this);
    }

    void StringInputExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression> const &
    StringInputExpression::getExpression() const
    {
        return m_expression;
    }

    std::string StringInputExpression::toString() const
    {
        return ""; // todo
    }
}