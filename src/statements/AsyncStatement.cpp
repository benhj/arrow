/// (c) Ben Jones 2019 - present

#include "AsyncStatement.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include "utility/ThreadManager.hpp"
#include <utility>

namespace arrow {
    AsyncStatement::AsyncStatement(long const lineNumber)
    : Statement(lineNumber)
    {
    }
    AsyncStatement & AsyncStatement::withToken(Token token)
    {
        m_keywordToken = std::move(token);
        return *this;
    }
    void AsyncStatement::addBodyStatement(std::shared_ptr<Statement> bodyStatement)
    {
        m_bodyStatements.emplace_back(std::move(bodyStatement));
    }

    std::vector<std::shared_ptr<Statement>>
    AsyncStatement::getBodyStatements() const
    {
        return m_bodyStatements;
    }

    std::string AsyncStatement::toString() const
    {
        return ""; // todo
    }

    std::shared_ptr<StatementEvaluator> AsyncStatement::getEvaluator() const
    {

        struct AsyncStatementEvaluator : public StatementEvaluator
        {
            explicit AsyncStatementEvaluator(AsyncStatement statement)
              : m_statement(std::move(statement))
            {
            }

           StatementResult evaluate(Environment & environment) const override
            {
                auto const bodyStatements = m_statement.getBodyStatements();
                ThreadManager::create([bodyStatements{std::move(bodyStatements)}, &environment] {
                    for(auto const & inner : bodyStatements) {
                        inner->getEvaluator()->evaluate(environment);
                    }
                });
                return StatementResult::Continue;
            }
          private:
            AsyncStatement m_statement;
        };

        return std::make_shared<AsyncStatementEvaluator>(*this);
    }
}