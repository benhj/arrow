#include "SimpleArrowStatementEvaluator.hpp"
#include "ExpressionEvaluator.hpp"
#include "expressions/IndexExpression.hpp"
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
    void SimpleArrowStatementEvaluator::evaluate(Cache & cache) const
    {
        auto const expression = m_statement.getExpression();
        auto evaluated = expression->getEvaluator()->evaluate(cache);
        auto const identifier = m_statement.getIdentifier();

        // first check if the target is an index expression rather
        // than an identifier
        auto indexExpression = m_statement.getIndexExpression();
        if(indexExpression) {
            auto casted = dynamic_cast<IndexExpression*>(indexExpression.get());
            auto containerIdentifier = casted->getIdentifierToken();
            auto exp = casted->getIndexExpression();
            auto indexEval = exp->getEvaluator()->evaluate(cache);
            if(indexEval.m_descriptor != TypeDescriptor::Int) {
                std::string error("Bad expression for index at line ");
                error.append(std::to_string(casted->getIdentifierToken().lineNumber));
                throw std::runtime_error(error);
            }
            auto index = std::get<int64_t>(indexEval.m_variantType);
            try {
                cache.setElementInContainer(containerIdentifier, index, evaluated);
            } catch (...) {
                std::string error("Index too big at line ");
                error.append(std::to_string(m_statement.getIdentifier().lineNumber));
                throw std::runtime_error(error);
            }
        } else {

            if(identifier.lexeme != Lexeme::DOLLAR_STRING) {
                cache.add(std::move(identifier), std::move(evaluated));
            } else {
                // Array handling
                if(cache.has(identifier)) {
                    auto orig = cache.get(identifier);
                    if(orig.m_descriptor == TypeDescriptor::Ints &&
                       evaluated.m_descriptor == TypeDescriptor::Int) {

                        add<int64_t>(std::move(orig), std::move(evaluated),
                                     cache, TypeDescriptor::Ints,
                                     std::move(identifier));

                    } else if(orig.m_descriptor == TypeDescriptor::Reals &&
                        evaluated.m_descriptor == TypeDescriptor::Real) {

                        add<long double>(std::move(orig), std::move(evaluated),
                                         cache, TypeDescriptor::Reals,
                                         std::move(identifier));

                    } else if(orig.m_descriptor == TypeDescriptor::Bools &&
                        evaluated.m_descriptor == TypeDescriptor::Bool) {

                        add<bool>(std::move(orig), std::move(evaluated),
                                  cache, TypeDescriptor::Bools,
                                  std::move(identifier));

                    } else if(orig.m_descriptor == TypeDescriptor::Strings &&
                        evaluated.m_descriptor == TypeDescriptor::String) {

                        add<std::string>(std::move(orig), std::move(evaluated),
                                         cache, TypeDescriptor::Strings,
                                         std::move(identifier));
                    }
                } else if(evaluated.m_descriptor == TypeDescriptor::Int) {

                    add<int64_t>(std::move(evaluated),
                                 cache, TypeDescriptor::Ints,
                                 std::move(identifier));

                } else if (evaluated.m_descriptor == TypeDescriptor::Real) {

                    add<long double>(std::move(evaluated),
                                     cache, TypeDescriptor::Reals,
                                     std::move(identifier));

                } else if (evaluated.m_descriptor == TypeDescriptor::Bool) {

                    add<bool>(std::move(evaluated),
                              cache, TypeDescriptor::Bools,
                              std::move(identifier));

                } else if (evaluated.m_descriptor == TypeDescriptor::String) {

                    add<std::string>(std::move(evaluated),
                                     cache, TypeDescriptor::Strings,
                                     std::move(identifier));

                }
            }
        }
    }
}