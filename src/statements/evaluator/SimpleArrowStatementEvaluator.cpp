/// (c) Ben Jones 2019

#include "SimpleArrowStatementEvaluator.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "expressions/IndexExpression.hpp"
#include "receivers/evaluator/ReceiverEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

namespace arrow {

    namespace {
        template <typename T>
        void add(Type container, Type evaluated, 
                 Environment & environment, TypeDescriptor const desc,
                 Token identifier) {

            auto vec = std::get<std::vector<T>>(container.m_variantType);
            auto deduced = std::get<T>(evaluated.m_variantType);
            vec.push_back(deduced);
            environment.add(std::move(identifier), {desc, vec});

        }

        template <typename T>
        void add(Type evaluated, Environment & environment, TypeDescriptor const desc,
                 Token identifier) {

            auto deduced = std::get<T>(evaluated.m_variantType);
            std::vector<T> vec;
            vec.push_back(deduced);
            environment.add(std::move(identifier), {desc, vec});

        }
    }

    SimpleArrowStatementEvaluator::SimpleArrowStatementEvaluator(SimpleArrowStatement statement)
      : m_statement(std::move(statement))
    {
    }
    StatementResult SimpleArrowStatementEvaluator::evaluate(Environment & environment) const
    {
        auto const expression = m_statement.getExpression();
        auto evaluated = expression->getEvaluator()->evaluate(environment);
        auto receiver = m_statement.getIdentifier();
        auto evaluator = receiver->getEvaluator();
        evaluator->evaluate(evaluated, environment);
        return StatementResult::Continue;
    }
}