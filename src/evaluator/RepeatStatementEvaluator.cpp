#include "RepeatStatementEvaluator.hpp"
#include "ExpressionEvaluator.hpp"
#include "statements/ElseIfStatement.hpp"
#include <utility>

namespace jasl {

    namespace {
        void evaluateBody(std::vector<std::shared_ptr<Statement>> bodyStatements,
                          Cache & cache)
        {
            for(auto const & statement : bodyStatements) {
                statement->getEvaluator()->evaluate(cache); 
            }
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

    void RepeatStatementEvaluator::evaluate(Cache & cache) const
    {
        auto expressionEvaluator = m_statement.getExpression()->getEvaluator();
        auto resolved = expressionEvaluator->evaluate(cache);
        if(!decaysToInt(resolved.m_descriptor)) {
            throw std::runtime_error("Bad type descriptor for repeat statement expression.");
        }
        int64_t val;
        if(resolved.m_descriptor == TypeDescriptor::Real) {
            auto result = std::get<long double>(resolved.m_variantType);
            val = static_cast<int64_t>(result);
        } else {
            val = std::get<int64_t>(resolved.m_variantType);
        }
        auto bodyStatements = m_statement.getBodyStatements();
        for(int64_t it = 0; it < val; ++it) {
            evaluateBody(bodyStatements, cache);
        }
    }
}