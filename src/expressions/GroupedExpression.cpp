/// (c) Ben Jones 2019 - present

#include "GroupedExpression.hpp"

namespace arrow {

    GroupedExpression::GroupedExpression(long const lineNumber) 
      : Expression(lineNumber)
      , m_expression{nullptr}
    {
    }

    std::shared_ptr<ExpressionEvaluator> GroupedExpression::getEvaluator() const
    {
        return m_expression->getEvaluator();
    }

    GroupedExpression & GroupedExpression::withExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
        return *this;
    }
    
    std::shared_ptr<Expression> const & GroupedExpression::getExpression() const
    {
        return m_expression;
    }
    std::string GroupedExpression::toString() const
    {
        std::string str("\nBegin grouped expression");
        str.append(m_expression->toString());
        str.append("\nEnd grouped expression");
        return str;
    }
}