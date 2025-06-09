/// (c) Ben Jones 2025 - present

#include "DrawCircleFunctionExpression.hpp"
#include "DrawCircleFunctionExpressionEvaluator.hpp"

namespace arrow {

    DrawCircleFunctionExpression::DrawCircleFunctionExpression(long const lineNumber)
      : Expression(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> DrawCircleFunctionExpression::getEvaluator() const
    {
        return std::make_shared<DrawCircleFunctionExpressionEvaluator>(*this);
    }

    void DrawCircleFunctionExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }

    std::shared_ptr<Expression> const &
    DrawCircleFunctionExpression::getExpression() const
    {
        return m_expression;
    }

    std::string DrawCircleFunctionExpression::toString() const
    {
        return ""; // todo
    }

} // namespace arrow