/// (c) Ben Jones 2019 - present

#include "AnsiStatement.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

namespace arrow {

    AnsiStatement::AnsiStatement(long const lineNumber,
                                 std::shared_ptr<ArrowlessStatement> statement)
    : Statement(lineNumber)
    , m_statement(std::move(statement))
    {

    }
    Token AnsiStatement::getToken() const
    {
        return m_statement->getToken();
    }
    std::shared_ptr<Expression> const & AnsiStatement::getExpression() const
    {
        return m_statement->getExpression();
    }
    std::string AnsiStatement::toString() const
    {
        return m_statement->toString();
    }

    std::shared_ptr<StatementEvaluator> AnsiStatement::getEvaluator() const
    {

        struct AnsiStatementEvaluator : public StatementEvaluator
        {
            explicit AnsiStatementEvaluator(AnsiStatement statement)
              : m_statement(std::move(statement))
            {
            }
            StatementResult evaluate(Environment & environment) const override
            {
                auto const expression = m_statement.getExpression();
                auto const type = expression->getEvaluator()->evaluate(environment);
                auto const token = m_statement.getToken();
                if(type.m_descriptor != TypeDescriptor::Int) {
                    throw LanguageException("Bad type", m_statement.getLineNumber());
                }
                auto casted = std::get<int64_t>(type.m_variantType);
                for(int64_t i = 0; i < casted; ++i) {
                    if(token.raw == "ansi_up") {
                        environment.getOutputStream()<<"\x1b[A";
                    } else if (token.raw == "ansi_down") {
                        // todo
                    }
                }
                return StatementResult::Continue;
            }
          private:
            AnsiStatement m_statement;
        };

        return std::make_shared<AnsiStatementEvaluator>(*this);
    }
}