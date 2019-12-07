/// (c) Ben Jones 2019 - present

#include "EchoStatement.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include <utility>

namespace arrow {

    EchoStatement::EchoStatement(long const lineNumber, std::shared_ptr<ArrowlessStatement> statement)
    : Statement(lineNumber)
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
            EchoStatementEvaluator(EchoStatement statement)
              : m_statement(std::move(statement))
            {
            }
            StatementResult evaluate(Cache & cache) const override
            {
                auto const expression = m_statement.getExpression();
                auto const type = expression->getEvaluator()->evaluate(cache);
                auto const token = m_statement.getToken();
                if(token.raw == "pr" || token.raw == "say") {
                    std::cout<<type.toString();
                } else {
                    std::cout<<type.toString()<<std::endl;
                }
                return StatementResult::Continue;
            }
          private:
            EchoStatement m_statement;
        };

        return std::make_shared<EchoStatementEvaluator>(*this);
    }
}