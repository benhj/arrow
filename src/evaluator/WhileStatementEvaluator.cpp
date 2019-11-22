#include "WhileStatementEvaluator.hpp"
#include "ExpressionEvaluator.hpp"
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

    WhileStatementEvaluator::WhileStatementEvaluator(WhileStatement statement)
      : m_statement(std::move(statement))
    {
    }

    void WhileStatementEvaluator::evaluate(Cache & cache) const
    {
        auto expressionEvaluator = m_statement.getExpression()->getEvaluator();
        auto resolved = expressionEvaluator->evaluate(cache);
        if(resolved.m_descriptor != TypeDescriptor::Bool) {
            throw std::runtime_error("Bad type descriptor for while statement expression.");
        }
        auto booleanVal = std::get<bool>(resolved.m_variantType);
        auto bodyStatements = m_statement.getBodyStatements();
        while(booleanVal) {
            evaluateBody(bodyStatements, cache);
            resolved = expressionEvaluator->evaluate(cache);
            booleanVal = std::get<bool>(resolved.m_variantType);
        }
    }
}