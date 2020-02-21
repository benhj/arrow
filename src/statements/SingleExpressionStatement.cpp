/// (c) Ben Jones 2019 - present

#include "SingleExpressionStatement.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include <utility>

namespace arrow {

    SingleExpressionStatement::SingleExpressionStatement(long const lineNumber,
                                                         std::ostream & os)
    : Statement(lineNumber, os)
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
        struct SingleExpressionStatementEvaluator : public StatementEvaluator
        {
            SingleExpressionStatementEvaluator(SingleExpressionStatement statement)
              : m_statement(std::move(statement))
            {}
            StatementResult evaluate(Environment & environment) const override
            {
                auto const expression = m_statement.getExpression();
                (void)expression->getEvaluator()->evaluate(environment);
                return StatementResult::Continue;
            }
          private:
            SingleExpressionStatement m_statement;
        };
        return std::make_shared<SingleExpressionStatementEvaluator>(*this);
    }
}