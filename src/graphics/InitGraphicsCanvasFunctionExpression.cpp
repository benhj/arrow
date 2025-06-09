/// (c) Ben Jones 2025 - present

#include "InitGraphicsCanvasFunctionExpression.hpp"
#include "InitGraphicsCanvasFunctionExpressionEvaluator.hpp"

namespace arrow {

    InitGraphicsCanvasFunctionExpression::InitGraphicsCanvasFunctionExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> InitGraphicsCanvasFunctionExpression::getEvaluator() const
    {
        return std::make_shared<InitGraphicsCanvasFunctionExpressionEvaluator>(*this);
    }

    void InitGraphicsCanvasFunctionExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression> const &
    InitGraphicsCanvasFunctionExpression::getExpression() const
    {
        return m_expression;
    }

    std::string InitGraphicsCanvasFunctionExpression::toString() const
    {
        return ""; // todo
    }
}