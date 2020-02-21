/// (c) Ben Jones 2019

#include "RepeatStatementEvaluator.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include "statements/ElseIfStatement.hpp"
#include "representation/real.hpp"
#include <utility>

namespace arrow {

    namespace {
        StatementResult evaluateBody(std::vector<std::shared_ptr<Statement>> bodyStatements,
                                     Environment & cache)
        {
            for(auto const & statement : bodyStatements) {
                auto const result = statement->getEvaluator()->evaluate(cache);
                if(result != StatementResult::Continue) {
                    return result;
                }
            }
            return StatementResult::Continue;
        }
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

    StatementResult RepeatStatementEvaluator::evaluate(Environment & cache) const
    {
        auto expressionEvaluator = m_statement.getExpression()->getEvaluator();
        auto resolved = expressionEvaluator->evaluate(cache);
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
            cache.pushEnvironmentLayer();
            evaluated = innerStatement->getEvaluator()->evaluate(cache);
            cache.popEnvironmentLayer();
            if(evaluated == StatementResult::Break) {
                break;
            } else if(evaluated == StatementResult::Return) {
                return evaluated;
            }
        }
        return evaluated;
    }
}