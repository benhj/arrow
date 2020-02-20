/// (c) Ben Jones 2020 - present

#include "TypeExpression.hpp"
#include "evaluator/TypeExpressionEvaluator.hpp"

namespace arrow {

    TypeExpression::TypeExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> TypeExpression::getEvaluator() const
    {
        return std::make_shared<TypeExpressionEvaluator>(*this);
    }

    void TypeExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression>
    TypeExpression::getExpression() const
    {
        return m_expression;
    }

    std::string TypeExpression::toString() const
    {
        return ""; // todo
    }
}