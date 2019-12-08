/// (c) Ben Jones 2019 - present

#include "MatchesExpression.hpp"
#include "evaluator/MatchesExpressionEvaluator.hpp"
#include <utility>

namespace arrow {

    MatchesExpression::MatchesExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }
    std::shared_ptr<ExpressionEvaluator> MatchesExpression::getEvaluator() const
    {
        return std::make_shared<MatchesExpressionEvaluator>(*this);
    }
    MatchesExpression & MatchesExpression::withLeftExpression(std::shared_ptr<Expression> expression)
    {
        m_left = std::move(expression);
        return *this;
    }
    MatchesExpression & MatchesExpression::withRightExpression(std::shared_ptr<Expression> expression)
    {
        m_right = std::move(expression);
        return *this;
    }
    std::shared_ptr<Expression> MatchesExpression::getLeftExpression() const
    {
        return m_left;
    }
    std::shared_ptr<Expression> MatchesExpression::getRightExpression() const
    {
        return m_right;
    }
    std::string MatchesExpression::toString() const
    {
        return ""; //todo
    }
}