#include "ForStatementEvaluator.hpp"
#include "ExpressionEvaluator.hpp"
#include "expressions/IdentifierExpression.hpp"
#include "statements/ElseIfStatement.hpp"
#include <utility>

namespace arrow {

    namespace {
        void evaluateBody(std::vector<std::shared_ptr<Statement>> bodyStatements,
                          Cache & cache)
        {
            for(auto const & statement : bodyStatements) {
                statement->getEvaluator()->evaluate(cache); 
            }
        }
    }

    ForStatementEvaluator::ForStatementEvaluator(ForStatement statement)
      : m_statement(std::move(statement))
    {
    }

    void ForStatementEvaluator::evaluate(Cache & cache) const
    {
        auto indexer = m_statement.getIndexer();
        auto identifier = m_statement.getIdentifier();
        IdentifierExpression exp;
        exp.withIdentifierToken(identifier);
        auto evaled = exp.getEvaluator()->evaluate(cache);
        if(evaled.m_descriptor != TypeDescriptor::List &&
           evaled.m_descriptor != TypeDescriptor::ExpressionCollection) {
            throw std::runtime_error("Bad type descriptor in for statement expression.");
        }
        auto value = std::get<std::vector<Type>>(evaled.m_variantType);
        auto bodyStatements = m_statement.getBodyStatements();
        for (auto const & element : value) {
            cache.pushCacheLayer();
            cache.add(indexer.raw, element);
            evaluateBody(bodyStatements, cache);
            cache.popCacheLayer();
        }
    }
}