/// (c) Ben Jones 2019

#include "WhileStatementEvaluator.hpp"
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
    }

    WhileStatementEvaluator::WhileStatementEvaluator(WhileStatement statement)
      : m_statement(std::move(statement))
    {
    }

    bool WhileStatementEvaluator::evaluate(Cache & cache) const
    {
        auto expressionEvaluator = m_statement.getExpression()->getEvaluator();
        auto resolved = expressionEvaluator->evaluate(cache);
        if(resolved.m_descriptor != TypeDescriptor::Bool) {
            throw LanguageException("Bad type or while statement expression",
                                    m_statement.getLineNumber());
        }
        auto booleanVal = std::get<bool>(resolved.m_variantType);
        auto bodyStatements = m_statement.getBodyStatements();
        while(booleanVal) {
            cache.pushCacheLayer();
            auto const evaluated = evaluateBody(bodyStatements, cache);
            cache.popCacheLayer();
            if(!evaluated) {
                return false;
            }
            resolved = expressionEvaluator->evaluate(cache);
            booleanVal = std::get<bool>(resolved.m_variantType);
        }
        return true;
    }
}