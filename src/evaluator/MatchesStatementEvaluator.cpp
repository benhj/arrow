#include "MatchesStatementEvaluator.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>
#include <vector>

namespace arrow {
    MatchesStatementEvaluator::MatchesStatementEvaluator(MatchesStatement statement)
      : m_statement(std::move(statement))
    {
    }
    void MatchesStatementEvaluator::evaluate(Cache & cache) const 
    {
        auto const left = m_statement.getLeftExpression();
        auto const evalLeft = left->getEvaluator()->evaluate(cache);
        if(evalLeft.m_descriptor != TypeDescriptor::List) {
            throw LanguageException("Left expression not a list",
                                    m_statement.getLineNumber());
        }
        auto const right = m_statement.getRightExpression();
        auto const evalRight = right->getEvaluator()->evaluate(cache);
        if(evalRight.m_descriptor != TypeDescriptor::List) {
            throw LanguageException("Right expression not a list",
                                    m_statement.getLineNumber());
        }

        auto const listLeft = std::get<std::vector<Type>>(evalLeft.m_variantType);
        auto const listRight = std::get<std::vector<Type>>(evalRight.m_variantType);
        auto const identifier = m_statement.getIdentifier();

        // Simple case -- direct match
        if(listLeft == listRight) {
            cache.add(identifier, {TypeDescriptor::Bool, true});
            return;
        }

        // Pattern matching

    }
}