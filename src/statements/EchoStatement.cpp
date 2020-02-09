/// (c) Ben Jones 2019 - present

#include "EchoStatement.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include <utility>

namespace arrow {

    EchoStatement::EchoStatement(long const lineNumber,
                                 std::shared_ptr<ArrowlessStatement> statement,
                                 std::ostream & os)
    : Statement(lineNumber, os)
    , m_statement(std::move(statement))
    {

    }
    Token EchoStatement::getToken() const
    {
        return m_statement->getToken();
    }
    std::shared_ptr<Expression> EchoStatement::getExpression() const
    {
        return m_statement->getExpression();
    }
    std::string EchoStatement::toString() const
    {
        return m_statement->toString();
    }

    std::shared_ptr<StatementEvaluator> EchoStatement::getEvaluator() const
    {
        struct EchoStatementEvaluator : public StatementEvaluator
        {
            EchoStatementEvaluator(EchoStatement statement,
                                   std::ostream & os)
              : m_statement(std::move(statement))
              , m_os(os)
            {
            }
            StatementResult evaluate(Cache & cache) const override
            {
                auto const expression = m_statement.getExpression();
                auto const type = expression->getEvaluator()->evaluate(cache);
                auto const token = m_statement.getToken();
                if(token.raw == "pr" || token.raw == "say") {
                    m_os<<type.toString()<<std::flush;
                } else {
                    m_os<<type.toString()<<std::endl;
                }
                return StatementResult::Continue;
            }
          private:
            EchoStatement m_statement;
            std::ostream & m_os;
        };

        return std::make_shared<EchoStatementEvaluator>(*this, m_os);
    }
}