/// (c) Ben Jones 2019 - present

#include "RandomFunctionExpression.hpp"
#include "evaluator/builtin/RandomFunctionExpressionEvaluator.hpp"

namespace arrow {

    RandomFunctionExpression::RandomFunctionExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> RandomFunctionExpression::getEvaluator() const
    {
        return std::make_shared<RandomFunctionExpressionEvaluator>(*this);
    }

    RandomFunctionExpression & 
    RandomFunctionExpression::withExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
        return *this;
    }
    std::shared_ptr<Expression>
    RandomFunctionExpression::getExpression() const
    {
        return m_expression;
    }

    std::string RandomFunctionExpression::toString() const
    {
        return ""; // todo
    }
}