/// (c) Ben Jones 2025 - present

#include "ToIntFunctionExpression.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "evaluator/ToIntFunctionExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <cmath>

namespace arrow {

    ToIntFunctionExpression::ToIntFunctionExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }
    std::shared_ptr<ExpressionEvaluator> ToIntFunctionExpression::getEvaluator() const
    {
        return std::make_shared<ToIntFunctionExpressionEvaluator>(*this);
    }

    void ToIntFunctionExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression> const &
    ToIntFunctionExpression::getExpression() const
    {
        return m_expression;
    }

    std::string ToIntFunctionExpression::toString() const
    {
        return ""; // todo
    }
}