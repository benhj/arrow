#include "IfStatementEvaluator.hpp"
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

    IfStatementEvaluator::IfStatementEvaluator(IfStatement statement)
      : m_statement(std::move(statement))
    {
    }

    void IfStatementEvaluator::evaluate(Cache & cache) const
    {
        auto ifEval = m_statement.getExpression()->getEvaluator();
        auto ifEvaluated = ifEval->evaluate(cache);
        if(ifEvaluated.m_descriptor != TypeDescriptor::Bool) {
            throw std::runtime_error("Bad type descriptor for if statement.");
        }
        auto theBool = std::get<bool>(ifEvaluated.m_variantType);
        if(theBool) {
            auto bodyStatements = m_statement.getBodyStatements();
            evaluateBody(std::move(bodyStatements), cache);
            return;
        } else {
            auto elseIfParts = m_statement.getElseIfParts();
            for(auto const & part : elseIfParts) {
                auto partExpressionEval = part->getExpression()->getEvaluator();
                auto partExpressionEvaluated = partExpressionEval->evaluate(cache);
                if(partExpressionEvaluated.m_descriptor != TypeDescriptor::Bool) {
                    throw std::runtime_error("Bad type descriptor for else part of if statement.");
                }
                auto partBool = std::get<bool>(partExpressionEvaluated.m_variantType);
                if(partBool) {
                    auto bodyStatements = part->getBodyStatements();
                    evaluateBody(std::move(bodyStatements), cache);
                    return;
                }
            }
            auto elsePart = m_statement.getElsePart();
            if(elsePart) {
                auto bodyStatements = elsePart->getBodyStatements();
                evaluateBody(std::move(bodyStatements), cache);
            }
        }
    }
}