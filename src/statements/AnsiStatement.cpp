#include "AnsiStatement.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
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
    std::shared_ptr<Expression> AnsiStatement::getExpression() const
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
            AnsiStatementEvaluator(AnsiStatement statement)
              : m_statement(std::move(statement))
            {
            }
            void evaluate(Cache & cache) const override
            {
                auto const expression = m_statement.getExpression();
                auto const type = expression->getEvaluator()->evaluate(cache);
                auto const token = m_statement.getToken();
                if(type.m_descriptor != TypeDescriptor::Int) {
                    throw LanguageException("Bad type", m_statement.getLineNumber());
                }
                auto casted = std::get<int64_t>(type.m_variantType);
                for(int64_t i = 0; i < casted; ++i) {
                    if(token.raw == "ansi_up") {
                        std::cout<<"\x1b[A";
                    } else if (token.raw == "ansi_down") {
                        // todo
                    }
                }
            }
          private:
            AnsiStatement m_statement;
        };

        return std::make_shared<AnsiStatementEvaluator>(*this);
    }
}