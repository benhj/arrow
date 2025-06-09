/// (c) Ben Jones 2025 - present

#include "SinFunctionExpression.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "evaluator/SinFunctionExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <cmath>

namespace arrow {

    SinFunctionExpression::SinFunctionExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }
    std::shared_ptr<ExpressionEvaluator> SinFunctionExpression::getEvaluator() const
    {
        return std::make_shared<SinFunctionExpressionEvaluator>(*this);
    }

    void SinFunctionExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression> const &
    SinFunctionExpression::getExpression() const
    {
        return m_expression;
    }

    std::string SinFunctionExpression::toString() const
    {
        return ""; // todo
    }
}