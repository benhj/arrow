#include "ReleaseStatement.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
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
    Token ReleaseStatement::getToken() const
    {
        return m_token;
    }
    Token ReleaseStatement::getIdentifier() const
    {
        return m_identifier;
    }
    std::string ReleaseStatement::toString() const
    {
        return ""; // todo
    }

    std::shared_ptr<StatementEvaluator> ReleaseStatement::getEvaluator() const
    {

        struct ReleaseStatementEvaluator : public StatementEvaluator
        {
            ReleaseStatementEvaluator(ReleaseStatement statement)
              : m_statement(std::move(statement))
            {
            }
            void evaluate(Cache & cache) const override
            {
                auto const identifier = m_statement.getIdentifier();
                if(cache.has(identifier)) {
                    cache.remove(identifier);
                }
            }
          private:
            ReleaseStatement m_statement;
        };

        return std::make_shared<ReleaseStatementEvaluator>(*this);
    }
}