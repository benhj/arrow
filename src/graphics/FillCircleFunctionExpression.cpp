/// (c) Ben Jones 2025 - present

#include "FillCircleFunctionExpression.hpp"
#include "FillCircleFunctionExpressionEvaluator.hpp" // Assuming this evaluator exists

namespace arrow {

    FillCircleFunctionExpression::FillCircleFunctionExpression(long const lineNumber)
      : Expression(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> FillCircleFunctionExpression::getEvaluator() const
    {
        return std::make_shared<FillCircleFunctionExpressionEvaluator>(*this);
    }

    void FillCircleFunctionExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }

    std::shared_ptr<Expression> const &
    FillCircleFunctionExpression::getExpression() const
    {
        return m_expression;
    }

    std::string FillCircleFunctionExpression::toString() const
    {
        return ""; // todo
    }

} // namespace arrow