/// (c) Ben Jones 2025 - present

#include "FillRectFunctionExpression.hpp"
#include "FillRectFunctionExpressionEvaluator.hpp" // Assuming this evaluator exists

namespace arrow {

    FillRectFunctionExpression::FillRectFunctionExpression(long const lineNumber)
      : Expression(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> FillRectFunctionExpression::getEvaluator() const
    {
        return std::make_shared<FillRectFunctionExpressionEvaluator>(*this);
    }

    void FillRectFunctionExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }

    std::shared_ptr<Expression> const &
    FillRectFunctionExpression::getExpression() const
    {
        return m_expression;
    }

    std::string FillRectFunctionExpression::toString() const
    {
        return ""; // todo
    }

} // namespace arrow