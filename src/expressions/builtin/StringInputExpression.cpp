/// (c) Ben Jones 2019 - present

#include "StringInputExpression.hpp"
#include "evaluator/builtin/StringInputExpressionEvaluator.hpp"

namespace arrow {

    StringInputExpression::StringInputExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> StringInputExpression::getEvaluator() const
    {
        return std::make_shared<StringInputExpressionEvaluator>(*this);
    }

    StringInputExpression & 
    StringInputExpression::withExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
        return *this;
    }
    std::shared_ptr<Expression>
    StringInputExpression::getExpression() const
    {
        return m_expression;
    }

    std::string StringInputExpression::toString() const
    {
        return ""; // todo
    }
}