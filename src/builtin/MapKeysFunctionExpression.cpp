/// (c) Ben Jones 2020 - present

#include "MapKeysFunctionExpression.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "evaluator/MapKeysFunctionExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <cmath>

namespace arrow {

    MapKeysFunctionExpression::MapKeysFunctionExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }
    std::shared_ptr<ExpressionEvaluator> MapKeysFunctionExpression::getEvaluator() const
    {
        return std::make_shared<MapKeysFunctionExpressionEvaluator>(*this);
    }

    void MapKeysFunctionExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression> const & 
    MapKeysFunctionExpression::getExpression() const
    {
        return m_expression;
    }

    std::string MapKeysFunctionExpression::toString() const
    {
        return ""; // todo
    }
}