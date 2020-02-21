/// (c) Ben Jones 2019 - present

#include "StartStatement.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include <utility>

namespace arrow {
    StartStatement::StartStatement(long const lineNumber,
                                   std::ostream & os)
    : Statement(lineNumber, os)
    {
    }
    StartStatement & StartStatement::withToken(Token token)
    {
        m_keywordToken = std::move(token);
        return *this;
    }
    StartStatement & StartStatement::withInnerStatement(std::shared_ptr<Statement> innerStatement)
    {
        m_innerStatement = std::move(innerStatement);
        return *this;
    }

    std::shared_ptr<Statement> StartStatement::getInnerStatement() const
    {
        return m_innerStatement;
    }

    std::string StartStatement::toString() const
    {
        return ""; // TODO
    }

    std::shared_ptr<StatementEvaluator> StartStatement::getEvaluator() const
    {

        struct StartStatementEvaluator : public StatementEvaluator
        {
            explicit StartStatementEvaluator(StartStatement statement)
              : m_statement(std::move(statement))
            {
            }

            StatementResult evaluate(Environment & environment) const override
            {
                auto innerStatement = m_statement.getInnerStatement();
                return innerStatement->getEvaluator()->evaluate(environment);
            }
          private:
            StartStatement m_statement;
        };

        return std::make_shared<StartStatementEvaluator>(*this);
    }
}