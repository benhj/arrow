/// (c) Ben Jones 2019 - present

#include "ReleaseStatement.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include <utility>

namespace arrow {

    ReleaseStatement::ReleaseStatement(long const lineNumber)
    : Statement(lineNumber)
    {

    }
    ReleaseStatement & ReleaseStatement::withToken(Token token)
    {
        m_token = std::move(token);
        return *this;
    }
    ReleaseStatement & ReleaseStatement::withIdentifier(Token token)
    {
        m_identifier = std::move(token);
        return *this;
    }
    Token const & ReleaseStatement::getToken() const
    {
        return m_token;
    }
    Token const & ReleaseStatement::getIdentifier() const
    {
        return m_identifier;
    }
    std::string ReleaseStatement::toString() const
    {
        return ""; // todo
    }

    std::shared_ptr<StatementEvaluator> ReleaseStatement::getEvaluator() const
    {

        struct ReleaseStatementEvaluator final : public StatementEvaluator
        {
            ReleaseStatementEvaluator(ReleaseStatement statement)
              : m_statement(std::move(statement))
            {
            }
            StatementResult evaluate(Environment & environment) const override
            {
                auto const & identifier = m_statement.getIdentifier().raw;
                if(environment.has(identifier)) {
                    environment.remove(identifier);
                }
                return StatementResult::Continue;
            }
          private:
            ReleaseStatement m_statement;
        };

        return std::make_shared<ReleaseStatementEvaluator>(*this);
    }
}