/// (c) Ben Jones 2025 - present

#include "GraphicsProcessFunctionExpression.hpp"
#include "GraphicsProcessFunctionExpressionEvaluator.hpp"

namespace arrow {

    GraphicsProcessFunctionExpression::GraphicsProcessFunctionExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> GraphicsProcessFunctionExpression::getEvaluator() const
    {
        return std::make_shared<GraphicsProcessFunctionExpressionEvaluator>(*this);
    }

    void GraphicsProcessFunctionExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression> const &
    GraphicsProcessFunctionExpression::getExpression() const
    {
        return m_expression;
    }

    std::string GraphicsProcessFunctionExpression::toString() const
    {
        return ""; // todo
    }
}