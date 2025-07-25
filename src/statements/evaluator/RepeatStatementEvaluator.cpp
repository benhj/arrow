/// (c) Ben Jones 2019 - present

#include "RepeatStatementEvaluator.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include "statements/ElseIfStatement.hpp"
#include "representation/real.hpp"
#include <utility>

namespace arrow {

    namespace {
        bool decaysToInt(TypeDescriptor const descriptor)
        {
            return descriptor == TypeDescriptor::Int ||
                   descriptor == TypeDescriptor::Real;
        }
    }

    RepeatStatementEvaluator::RepeatStatementEvaluator(RepeatStatement statement)
      : m_statement(std::move(statement))
    {
    }

    StatementResult RepeatStatementEvaluator::evaluate(Environment & environment) const
    {
        auto expressionEvaluator = m_statement.getExpression()->getEvaluator();
        auto resolved = expressionEvaluator->evaluate(environment);
        if(!decaysToInt(resolved.m_descriptor)) {
            throw LanguageException("Bad type for repeat statement", m_statement.getLineNumber());
        }
        int64_t val;
        if(resolved.m_descriptor == TypeDescriptor::Real) {
            auto result = std::get<real>(resolved.m_variantType);
            val = static_cast<int64_t>(result);
        } else {
            val = std::get<int64_t>(resolved.m_variantType);
        }

        auto innerStatement = m_statement.getInnerStatement();
        auto evaluated = StatementResult::Continue;
        for(int64_t it = 0; it < val; ++it) {
            environment.pushEnvironmentLayer();
            evaluated = innerStatement->getEvaluator()->evaluate(environment);
            environment.popEnvironmentLayer();
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