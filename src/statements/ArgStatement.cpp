/// (c) Ben Jones 2019 - present

#include "ArgStatement.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

namespace arrow {
    
    ArgStatement::ArgStatement(long const lineNumber, ArrowStatement arrowStatement) 
      : Statement(lineNumber)
      , m_arrowStatement(std::move(arrowStatement))
    {
    }

    std::string ArgStatement::toString() const
    {
        return m_arrowStatement.toString();
    }

    std::shared_ptr<StatementEvaluator> ArgStatement::getEvaluator() const
    {
        struct ArgStatementEvaluator : StatementEvaluator
        {
            explicit ArgStatementEvaluator(ArgStatement statement)
              : m_statement(std::move(statement))
            {
            }

            StatementResult evaluate(Cache & cache) const override
            {
                auto evaluator = m_statement.getExpression()->getEvaluator();
                auto evaluated = evaluator->evaluate(cache);
                if(evaluated.m_descriptor != TypeDescriptor::Int) {
                    throw LanguageException("Arg parsing broke", m_statement.getLineNumber());
                }
                auto const index = std::get<int64_t>(evaluated.m_variantType);
                auto progArg = cache.getProgramArgument(index);
                auto identifier = m_statement.getIdentifier();
                cache.add(identifier, progArg);
                return StatementResult::Continue;
            }

          private:
            ArgStatement m_statement;
        };
        return std::make_shared<ArgStatementEvaluator>(*this);
    }

    ArgStatement const & ArgStatement::withToken(Token token) const
    {
        m_arrowStatement.withToken(std::move(token));
        return *this;
    }

    Token ArgStatement::getToken() const
    {
        return m_arrowStatement.getToken();
    }

    std::shared_ptr<Expression> ArgStatement::getExpression() const
    {
        return m_arrowStatement.getExpression();
    }

    Token ArgStatement::getIdentifier() const
    {
        return m_arrowStatement.getIdentifier();
    }

    ArrowStatement ArgStatement::getArrowStatement() const
    {
        return m_arrowStatement;
    }
}