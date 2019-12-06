/// (c) Ben Jones 2019

#include "RepeatStatementEvaluator.hpp"
#include "ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include "statements/ElseIfStatement.hpp"
#include <utility>

namespace arrow {

    namespace {
        bool evaluateBody(std::vector<std::shared_ptr<Statement>> bodyStatements,
                          Cache & cache)
        {
            for(auto const & statement : bodyStatements) {
                if(!statement->getEvaluator()->evaluate(cache)) {
                    return false;
                }
            }
            return true;
        }
        bool decaysToInt(TypeDescriptor const descriptor)
        {
            return descriptor == TypeDescriptor::Int ||
                   descriptor == TypeDescriptor::Real;
        }
    }

    RepeatStatementEvaluator::RepeatStatementEvaluator(RepeatStatement statement)
      : m_statement(std::move(statement))
    {
    }

    bool RepeatStatementEvaluator::evaluate(Cache & cache) const
    {
        auto expressionEvaluator = m_statement.getExpression()->getEvaluator();
        auto resolved = expressionEvaluator->evaluate(cache);
        if(!decaysToInt(resolved.m_descriptor)) {
            throw LanguageException("Bad type for repeat statement", m_statement.getLineNumber());
        }
        int64_t val;
        if(resolved.m_descriptor == TypeDescriptor::Real) {
            auto result = std::get<long double>(resolved.m_variantType);
            val = static_cast<int64_t>(result);
        } else {
            val = std::get<int64_t>(resolved.m_variantType);
        }
        cache.pushCacheLayer();
        auto bodyStatements = m_statement.getBodyStatements();
        auto evaluated = true;
        for(int64_t it = 0; it < val; ++it) {
            if(!evaluateBody(bodyStatements, cache)) {
                evaluated = false;
                break;
            }
        }
        cache.popCacheLayer();
        return evaluated;
    }
}