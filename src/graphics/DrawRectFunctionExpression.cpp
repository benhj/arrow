/// (c) Ben Jones 2025 - present

#include "DrawRectFunctionExpression.hpp"
#include "DrawRectFunctionExpressionEvaluator.hpp" // Assuming this evaluator exists

namespace arrow {

    DrawRectFunctionExpression::DrawRectFunctionExpression(long const lineNumber)
      : Expression(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> DrawRectFunctionExpression::getEvaluator() const
    {
        return std::make_shared<DrawRectFunctionExpressionEvaluator>(*this);
    }

    void DrawRectFunctionExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }

    std::shared_ptr<Expression> const &
    DrawRectFunctionExpression::getExpression() const
    {
        return m_expression;
    }

    std::string DrawRectFunctionExpression::toString() const
    {
        return ""; // todo
    }

} // namespace arrow