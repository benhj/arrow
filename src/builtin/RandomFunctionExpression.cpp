/// (c) Ben Jones 2019 - present

#include "RandomFunctionExpression.hpp"
#include "evaluator/RandomFunctionExpressionEvaluator.hpp"

namespace arrow {

    RandomFunctionExpression::RandomFunctionExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> RandomFunctionExpression::getEvaluator() const
    {
        return std::make_shared<RandomFunctionExpressionEvaluator>(*this);
    }

    void RandomFunctionExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression> const &
    RandomFunctionExpression::getExpression() const
    {
        return m_expression;
    }

    std::string RandomFunctionExpression::toString() const
    {
        return ""; // todo
    }
}