/// (c) Ben Jones 2025 - present

#include "Atan2FunctionExpression.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "evaluator/Atan2FunctionExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <cmath>

namespace arrow {

    Atan2FunctionExpression::Atan2FunctionExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }
    std::shared_ptr<ExpressionEvaluator> Atan2FunctionExpression::getEvaluator() const
    {
        return std::make_shared<Atan2FunctionExpressionEvaluator>(*this);
    }

    void Atan2FunctionExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression> const &
    Atan2FunctionExpression::getExpression() const
    {
        return m_expression;
    }

    std::string Atan2FunctionExpression::toString() const
    {
        return ""; // todo
    }
}