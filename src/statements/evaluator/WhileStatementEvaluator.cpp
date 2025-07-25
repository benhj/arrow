/// (c) Ben Jones 2019 - present

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

    StatementResult WhileStatementEvaluator::evaluate(Environment & environment) const
    {
        auto expressionEvaluator = m_statement.getExpression()->getEvaluator();
        auto resolved = expressionEvaluator->evaluate(environment);
        if(resolved.m_descriptor != TypeDescriptor::Bool) {
            throw LanguageException("Bad type or while statement expression",
                                    m_statement.getLineNumber());
        }
        auto booleanVal = std::get<bool>(resolved.m_variantType);
        auto innerStatement = m_statement.getInnerStatement();
        while(booleanVal) {
            environment.pushEnvironmentLayer();
            auto evaluated = innerStatement->getEvaluator()->evaluate(environment);
            environment.popEnvironmentLayer();
            resolved = expressionEvaluator->evaluate(environment);
            booleanVal = std::get<bool>(resolved.m_variantType);
            if(evaluated == StatementResult::Break) {
                break;
            } else if(evaluated == StatementResult::LoopContinue) {
                continue;
            } else if(evaluated == StatementResult::Return ||
                      evaluated == StatementResult::Exit) {
                return evaluated;
            }
        }
        return StatementResult::Continue;
    }
}