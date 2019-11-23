#include "FunctionStatementEvaluator.hpp"
#include <utility>

namespace arrow {
    FunctionStatementEvaluator::FunctionStatementEvaluator(FunctionStatement statement)
      : m_statement(std::move(statement))
    {
    }
    void FunctionStatementEvaluator::evaluate(Cache & cache) const
    {
        auto const name = m_statement.getName().raw;
        auto const statements = m_statement.getBodyStatements();
        cache.pushCacheLayer();
        for(auto const & statement : statements) {
            statement->getEvaluator()->evaluate(cache);
        }
        auto const returnIdentifier = m_statement.getReturnIdentifier().raw;
        if(!cache.has(returnIdentifier)) {
            throw std::runtime_error("Can't find return value in function statement");
        }
        auto const value = cache.get(returnIdentifier);
        cache.pushReturnValue(value);
        cache.popCacheLayer();
    }
}