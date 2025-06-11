/// (c) Ben Jones 2025 - present

#include "ExpFunctionExpression.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "evaluator/ExpFunctionExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <cmath>

namespace arrow {

    ExpFunctionExpression::ExpFunctionExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }
    std::shared_ptr<ExpressionEvaluator> ExpFunctionExpression::getEvaluator() const
    {
        return std::make_shared<ExpFunctionExpressionEvaluator>(*this);
    }

    void ExpFunctionExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression> const &
    ExpFunctionExpression::getExpression() const
    {
        return m_expression;
    }

    std::string ExpFunctionExpression::toString() const
    {
        return ""; // todo
    }
}