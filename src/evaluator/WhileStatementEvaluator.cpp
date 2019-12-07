/// (c) Ben Jones 2019

#include "WhileStatementEvaluator.hpp"
#include "ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include "statements/ElseIfStatement.hpp"
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

    WhileStatementEvaluator::WhileStatementEvaluator(WhileStatement statement)
      : m_statement(std::move(statement))
    {
    }

    StatementResult WhileStatementEvaluator::evaluate(Cache & cache) const
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
            if(evaluated != StatementResult::Continue) {
                return evaluated;
            }
            resolved = expressionEvaluator->evaluate(cache);
            booleanVal = std::get<bool>(resolved.m_variantType);
        }
        return StatementResult::Continue;
    }
}