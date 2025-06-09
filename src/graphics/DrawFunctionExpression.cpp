/// (c) Ben Jones 2025 - present

#include "DrawFunctionExpression.hpp"
#include "DrawFunctionExpressionEvaluator.hpp"

namespace arrow {

    DrawFunctionExpression::DrawFunctionExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> DrawFunctionExpression::getEvaluator() const
    {
        return std::make_shared<DrawFunctionExpressionEvaluator>(*this);
    }

    void DrawFunctionExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression> const &
    DrawFunctionExpression::getExpression() const
    {
        return m_expression;
    }

    std::string DrawFunctionExpression::toString() const
    {
        return ""; // todo
    }
}