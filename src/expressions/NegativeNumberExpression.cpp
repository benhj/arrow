/// (c) Ben Jones 2020 - present

#include "NegativeNumberExpression.hpp"
#include "evaluator/NegativeNumberEvaluator.hpp"

namespace arrow {

    NegativeNumberExpression::NegativeNumberExpression(long const lineNumber) 
      : Expression(lineNumber)
      , m_expression{nullptr}
    {
    }

    std::shared_ptr<ExpressionEvaluator> NegativeNumberExpression::getEvaluator() const
    {
        return std::make_shared<NegativeNumberEvaluator>(m_expression->getEvaluator(), m_lineNumber);
    }

    NegativeNumberExpression & NegativeNumberExpression::withExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
        return *this;
    }

    std::shared_ptr<Expression> const & NegativeNumberExpression::getExpression() const
    {
        return m_expression;
    }
    std::string NegativeNumberExpression::toString() const
    {
        return ""; // TODO
    }
}