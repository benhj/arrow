/// (c) Ben Jones 2019

#include "IfStatementEvaluator.hpp"
#include "ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include "statements/ElseIfStatement.hpp"
#include <utility>

namespace arrow {

    IfStatementEvaluator::IfStatementEvaluator(IfStatement statement)
      : m_statement(std::move(statement))
    {
    }

    StatementResult IfStatementEvaluator::evaluate(Cache & cache) const
    {
        auto ifEval = m_statement.getExpression()->getEvaluator();
        auto ifEvaluated = ifEval->evaluate(cache);
        if(ifEvaluated.m_descriptor != TypeDescriptor::Bool) {
            throw LanguageException("Bad type for if statement", m_statement.getLineNumber());
        }
        auto theBool = std::get<bool>(ifEvaluated.m_variantType);
        if(theBool) {
            auto innerStatement = m_statement.getInnerStatement();
            cache.pushCacheLayer();
            auto res = innerStatement->getEvaluator()->evaluate(cache);
            cache.popCacheLayer();
            return res;
        } else {
            auto elseIfParts = m_statement.getElseIfParts();
            for(auto const & part : elseIfParts) {
                auto partExpressionEval = part->getExpression()->getEvaluator();
                auto partExpressionEvaluated = partExpressionEval->evaluate(cache);
                if(partExpressionEvaluated.m_descriptor != TypeDescriptor::Bool) {
                    throw LanguageException("Bad type for else statement", part->getLineNumber());
                }
                auto partBool = std::get<bool>(partExpressionEvaluated.m_variantType);
                if(partBool) {
                    auto innerStatement = part->getInnerStatement();
                    cache.pushCacheLayer();
                    auto res = innerStatement->getEvaluator()->evaluate(cache);
                    cache.popCacheLayer();
                    return res;
                }
            }
            auto elsePart = m_statement.getElsePart();
            if(elsePart) {
                auto innerStatement = elsePart->getInnerStatement();
                cache.pushCacheLayer();
                auto res = innerStatement->getEvaluator()->evaluate(cache);
                cache.popCacheLayer();
                return res;
            }
        }
        return StatementResult::Continue;
    }
}