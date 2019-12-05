/// (c) Ben Jones 2019

#include "IfStatementEvaluator.hpp"
#include "ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
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

    IfStatementEvaluator::IfStatementEvaluator(IfStatement statement)
      : m_statement(std::move(statement))
    {
    }

    void IfStatementEvaluator::evaluate(Cache & cache) const
    {
        auto ifEval = m_statement.getExpression()->getEvaluator();
        auto ifEvaluated = ifEval->evaluate(cache);
        if(ifEvaluated.m_descriptor != TypeDescriptor::Bool) {
            throw LanguageException("Bad type for if statement", m_statement.getLineNumber());
        }
        auto theBool = std::get<bool>(ifEvaluated.m_variantType);
        if(theBool) {
            cache.pushCacheLayer();
            auto bodyStatements = m_statement.getBodyStatements();
            evaluateBody(std::move(bodyStatements), cache);
            cache.popCacheLayer();
            return;
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
                    cache.pushCacheLayer();
                    auto bodyStatements = part->getBodyStatements();
                    evaluateBody(std::move(bodyStatements), cache);
                    cache.popCacheLayer();
                    return;
                }
            }
            auto elsePart = m_statement.getElsePart();
            if(elsePart) {
                cache.pushCacheLayer();
                auto bodyStatements = elsePart->getBodyStatements();
                evaluateBody(std::move(bodyStatements), cache);
                cache.popCacheLayer();
            }
        }
    }
}