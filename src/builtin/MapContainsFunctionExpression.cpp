/// (c) Ben Jones 2020 - present

#include "MapContainsFunctionExpression.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "evaluator/MapContainsFunctionExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <cmath>

namespace arrow {

    MapContainsFunctionExpression::MapContainsFunctionExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }
    std::shared_ptr<ExpressionEvaluator> MapContainsFunctionExpression::getEvaluator() const
    {
        return std::make_shared<MapContainsFunctionExpressionEvaluator>(*this);
    }

    void MapContainsFunctionExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression> const & 
    MapContainsFunctionExpression::getExpression() const
    {
        return m_expression;
    }

    std::string MapContainsFunctionExpression::toString() const
    {
        return ""; // todo
    }
}