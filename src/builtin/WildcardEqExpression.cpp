/// (c) Ben Jones 2020 - present

#include "WildcardEqExpression.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "evaluator/WildcardEqExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <cmath>

namespace arrow {

    WildcardEqExpression::WildcardEqExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }
    std::shared_ptr<ExpressionEvaluator> WildcardEqExpression::getEvaluator() const
    {
        return std::make_shared<WildcardEqExpressionEvaluator>(*this);
    }

    void WildcardEqExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression>
    WildcardEqExpression::getExpression() const
    {
        return m_expression;
    }

    std::string WildcardEqExpression::toString() const
    {
        return ""; // todo
    }
}