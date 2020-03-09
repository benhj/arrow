/// (c) Ben Jones 2019 - present

#include "FunctionExpression.hpp"
#include "evaluator/FunctionExpressionEvaluator.hpp"

namespace arrow {

    FunctionExpression::FunctionExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> FunctionExpression::getEvaluator() const
    {
        return std::make_shared<FunctionExpressionEvaluator>(*this);
    }

    FunctionExpression & FunctionExpression::withFunctionNameToken(Token token)
    {
        m_nameToken = std::move(token);
        return *this;
    }
    FunctionExpression & 
    FunctionExpression::withExpressionCollection(std::shared_ptr<Expression> expression)
    {
        m_expressionCollection = std::move(expression);
        return *this;
    }
    std::shared_ptr<Expression> const &
    FunctionExpression::getExpressionCollection() const
    {
        return m_expressionCollection;
    }

    Token const & FunctionExpression::getName() const
    {
        return m_nameToken;
    }

    std::string FunctionExpression::toString() const
    {
        return ""; // todo
    }
}