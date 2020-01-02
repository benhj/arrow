/// (c) Ben Jones 2019 - present

#include "ScopedBlockStatementEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

namespace arrow {

    namespace {
        StatementResult evaluateBody(std::vector<std::shared_ptr<Statement>> bodyStatements,
                                     Cache & cache)
        {
            for(auto const & statement : bodyStatements) {
                auto const result = statement->getEvaluator()->evaluate(cache);
                if(result != StatementResult::Continue) {
                    return result;
                }
            }
            return StatementResult::Continue;
        }
    }

    ScopedBlockStatementEvaluator::ScopedBlockStatementEvaluator(ScopedBlockStatement statement)
      : m_statement(std::move(statement))
    {
    }

    StatementResult ScopedBlockStatementEvaluator::evaluate(Cache & cache) const
    {
        auto bodyStatements = m_statement.getBodyStatements();
        auto const evaluated = evaluateBody(bodyStatements, cache);
        return evaluated;
    }
}