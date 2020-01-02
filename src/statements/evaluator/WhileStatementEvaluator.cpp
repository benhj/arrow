/// (c) Ben Jones 2019

#include "WhileStatementEvaluator.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include "statements/ElseIfStatement.hpp"
#include <utility>

namespace arrow {

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
        auto innerStatement = m_statement.getInnerStatement();
        while(booleanVal) {
            cache.pushCacheLayer();
            auto evaluated = innerStatement->getEvaluator()->evaluate(cache);
            cache.popCacheLayer();
            if(evaluated == StatementResult::Break) {
                break;
            } else if(evaluated == StatementResult::Return) {
                return evaluated;
            }
            resolved = expressionEvaluator->evaluate(cache);
            booleanVal = std::get<bool>(resolved.m_variantType);
        }
        return StatementResult::Continue;
    }
}