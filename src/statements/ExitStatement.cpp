/// (c) Ben Jones 2019 - present

#include "ExitStatement.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include <utility>

namespace arrow {

    ExitStatement::ExitStatement(long const lineNumber,
                                 std::shared_ptr<ArrowlessStatement> statement)
    : Statement(lineNumber)
    , m_statement(std::move(statement))
    {

    }
    Token ExitStatement::getToken() const
    {
        return m_statement->getToken();
    }
    std::shared_ptr<Expression> ExitStatement::getExpression() const
    {
        return m_statement->getExpression();
    }
    std::string ExitStatement::toString() const
    {
        return m_statement->toString();
    }

    std::shared_ptr<StatementEvaluator> ExitStatement::getEvaluator() const
    {

        struct ExitStatementEvaluator : public StatementEvaluator
        {
            ExitStatementEvaluator(ExitStatement statement)
              : m_statement(std::move(statement))
            {
            }
            bool evaluate(Cache & cache) const override
            {
                exit(0);
                return true;
            }
          private:
            ExitStatement m_statement;
        };

        return std::make_shared<ExitStatementEvaluator>(*this);
    }
}