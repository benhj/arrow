/// (c) Ben Jones 2020 - present

#include "RegexFunctionExpression.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "evaluator/RegexFunctionExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <cmath>

namespace arrow {

    RegexFunctionExpression::RegexFunctionExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }
    std::shared_ptr<ExpressionEvaluator> RegexFunctionExpression::getEvaluator() const
    {
        return std::make_shared<RegexFunctionExpressionEvaluator>(*this);
    }

    void RegexFunctionExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression> const &
    RegexFunctionExpression::getExpression() const
    {
        return m_expression;
    }

    std::string RegexFunctionExpression::toString() const
    {
        return ""; // todo
    }
}