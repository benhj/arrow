/// (c) Ben Jones 2019 - present

#include "EraseStatement.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include "expressions/IndexExpression.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

namespace arrow {

    EraseStatement::EraseStatement(long const lineNumber, std::shared_ptr<ArrowlessStatement> statement)
    : Statement(lineNumber)
    , m_statement(std::move(statement))
    {

    }
    Token EraseStatement::getToken() const
    {
        return m_statement->getToken();
    }
    std::shared_ptr<Expression> EraseStatement::getExpression() const
    {
        return m_statement->getExpression();
    }
    std::string EraseStatement::toString() const
    {
        return m_statement->toString();
    }

    std::shared_ptr<StatementEvaluator> EraseStatement::getEvaluator() const
    {

        struct EraseStatementEvaluator : public StatementEvaluator
        {
            EraseStatementEvaluator(EraseStatement statement)
              : m_statement(std::move(statement))
            {
            }
            StatementResult evaluate(Cache & cache) const override
            {
                auto casted = dynamic_cast<IndexExpression*>(m_statement.getExpression().get());
                auto containerIdentifier = casted->getIdentifierToken();
                auto exp = casted->getIndexExpression();
                auto indexEval = exp->getEvaluator()->evaluate(cache);
                if(indexEval.m_descriptor != TypeDescriptor::Int) {
                    throw LanguageException("Bad expression for index",
                                            m_statement.getLineNumber());
                }
                auto index = std::get<int64_t>(indexEval.m_variantType);
                try {
                    cache.eraseElementInContainer(containerIdentifier.raw, index);
                } catch (...) {
                    throw LanguageException("Problem erasing element",
                                            m_statement.getLineNumber());
                }
                return StatementResult::Continue;
            }
          private:
            EraseStatement m_statement;
        };

        return std::make_shared<EraseStatementEvaluator>(*this);
    }
}