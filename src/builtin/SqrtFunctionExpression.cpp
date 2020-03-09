/// (c) Ben Jones 2019 - present

#include "SqrtFunctionExpression.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "evaluator/SqrtFunctionExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <cmath>

namespace arrow {

    SqrtFunctionExpression::SqrtFunctionExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }
    std::shared_ptr<ExpressionEvaluator> SqrtFunctionExpression::getEvaluator() const
    {
        return std::make_shared<SqrtFunctionExpressionEvaluator>(*this);
    }

    void SqrtFunctionExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression> const &
    SqrtFunctionExpression::getExpression() const
    {
        return m_expression;
    }

    std::string SqrtFunctionExpression::toString() const
    {
        return ""; // todo
    }
}