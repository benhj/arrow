/// (c) Ben Jones 2025 - present

#include "CosFunctionExpression.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "evaluator/CosFunctionExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <cmath>

namespace arrow {

    CosFunctionExpression::CosFunctionExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }
    std::shared_ptr<ExpressionEvaluator> CosFunctionExpression::getEvaluator() const
    {
        return std::make_shared<CosFunctionExpressionEvaluator>(*this);
    }

    void CosFunctionExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression> const &
    CosFunctionExpression::getExpression() const
    {
        return m_expression;
    }

    std::string CosFunctionExpression::toString() const
    {
        return ""; // todo
    }
}