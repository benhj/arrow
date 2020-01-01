/// (c) Ben Jones 2019

#include "SimpleArrowStatementEvaluator.hpp"
#include "ExpressionEvaluator.hpp"
#include "expressions/IndexExpression.hpp"
#include "evaluator/receivers/ReceiverEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

namespace arrow {

    namespace {
        template <typename T>
        void add(Type container, Type evaluated, 
                 Cache & cache, TypeDescriptor const desc,
                 Token identifier) {

            auto vec = std::get<std::vector<T>>(container.m_variantType);
            auto deduced = std::get<T>(evaluated.m_variantType);
            vec.push_back(deduced);
            cache.add(std::move(identifier), {desc, vec});

        }

        template <typename T>
        void add(Type evaluated, Cache & cache, TypeDescriptor const desc,
                 Token identifier) {

            auto deduced = std::get<T>(evaluated.m_variantType);
            std::vector<T> vec;
            vec.push_back(deduced);
            cache.add(std::move(identifier), {desc, vec});

        }
    }

    SimpleArrowStatementEvaluator::SimpleArrowStatementEvaluator(SimpleArrowStatement statement)
      : m_statement(std::move(statement))
    {
    }
    StatementResult SimpleArrowStatementEvaluator::evaluate(Cache & cache) const
    {
        auto const expression = m_statement.getExpression();
        auto evaluated = expression->getEvaluator()->evaluate(cache);
        auto receiver = m_statement.getIdentifier();
        auto evaluator = receiver->getEvaluator();
        evaluator->evaluate(evaluated, cache);
        return StatementResult::Continue;
    }
}