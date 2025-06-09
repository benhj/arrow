/// (c) Ben Jones 2025 - present

#include "DrawLineFunctionExpression.hpp"
#include "DrawLineFunctionExpressionEvaluator.hpp"

namespace arrow {

    DrawLineFunctionExpression::DrawLineFunctionExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> DrawLineFunctionExpression::getEvaluator() const
    {
        return std::make_shared<DrawLineFunctionExpressionEvaluator>(*this);
    }

    void DrawLineFunctionExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression> const &
    DrawLineFunctionExpression::getExpression() const
    {
        return m_expression;
    }

    std::string DrawLineFunctionExpression::toString() const
    {
        return ""; // todo
    }
}