/// (c) Ben Jones 2020 - present

#include "TokenizeFunctionExpression.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "evaluator/TokenizeFunctionExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <cmath>

namespace arrow {

    TokenizeFunctionExpression::TokenizeFunctionExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }
    std::shared_ptr<ExpressionEvaluator> TokenizeFunctionExpression::getEvaluator() const
    {
        return std::make_shared<TokenizeFunctionExpressionEvaluator>(*this);
    }

    void TokenizeFunctionExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression> const &
    TokenizeFunctionExpression::getExpression() const
    {
        return m_expression;
    }

    std::string TokenizeFunctionExpression::toString() const
    {
        return ""; // todo
    }
}