/// (c) Ben Jones 2019 - present

#include "TrimStringFunctionExpression.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "evaluator/TrimStringFunctionExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <cmath>

namespace arrow {

    TrimStringFunctionExpression::TrimStringFunctionExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }
    std::shared_ptr<ExpressionEvaluator> TrimStringFunctionExpression::getEvaluator() const
    {
        return std::make_shared<TrimStringFunctionExpressionEvaluator>(*this);
    }

    void TrimStringFunctionExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression> const &
    TrimStringFunctionExpression::getExpression() const
    {
        return m_expression;
    }

    std::string TrimStringFunctionExpression::toString() const
    {
        return ""; // todo
    }
}