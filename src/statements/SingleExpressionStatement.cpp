#include "SingleExpressionStatement.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include <utility>

namespace arrow {

    SingleExpressionStatement::SingleExpressionStatement(long const lineNumber) 
      : Statement(lineNumber)
    {
    }
    SingleExpressionStatement& 
    SingleExpressionStatement::withExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
        return *this;
    }
    std::shared_ptr<Expression> SingleExpressionStatement::getExpression() const
    {
        return m_expression;
    }

    std::string SingleExpressionStatement::toString() const
    {
        return ""; // todo
    }

    std::shared_ptr<StatementEvaluator> SingleExpressionStatement::getEvaluator() const
    {
        return nullptr;
    }
}