/// (c) Ben Jones 2019 - present

#include "DollarIdentifierReceiverEvaluator.hpp"
#include "expressions/IdentifierExpression.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

namespace arrow {

    namespace {
        template <typename T>
        void add(Type evaluated, Environment & environment, std::string identifier) {
            environment.pushBackContainerElement(std::move(identifier), evaluated);
        }

        void addIntToRealVector(Type evaluated, Environment & environment, std::string identifier) {
            // This is always assumed to succeed
            auto found = environment.findAndRetrieveCached(std::move(identifier));
            auto & vec = std::get<std::vector<real>>(found->second.m_variantType);
            auto deduced = std::get<int64_t>(evaluated.m_variantType);
            vec.push_back(deduced);
        }    

        void addToString(Type evaluated, Environment & environment, std::string identifier) {

            // This is always assumed to succeed
            auto found = environment.findAndRetrieveCached(std::move(identifier));
            auto & vec = std::get<std::string>(found->second.m_variantType);
            auto deduced = std::get<char>(evaluated.m_variantType);
            vec.push_back(deduced);
        }

        void addToList(Type evaluated, Environment & environment, std::string identifier) {
            environment.pushBackContainerElement(std::move(identifier), evaluated);
        }

        template <typename T>
        void add(Type evaluated, Environment & environment, TypeDescriptor const desc,
                 std::string identifier) {

            auto deduced = std::get<T>(evaluated.m_variantType);
            std::vector<T> vec;
            vec.push_back(deduced);
            environment.add(std::move(identifier), {desc, vec});
        }
    }

    DollarIdentifierReceiverEvaluator::DollarIdentifierReceiverEvaluator(std::shared_ptr<Expression> expression) 
      : m_expression(std::move(expression))
    {
    }

    void DollarIdentifierReceiverEvaluator::evaluate(Type evaluated, Environment & environment) const
    {
        // A bit hacky -- figure out what the exression type is
        // based on the expression type string.
        char const * expressionType = m_expression->getTypeString();
        if(std::strcmp(expressionType, "Identifier") == 0) {
            auto expr = dynamic_cast<IdentifierExpression*>(m_expression.get());
            handleIdentifierExpression(std::move(evaluated), environment, std::move(expr));
            return;
        }
        throw LanguageException("Expression type not compatible", m_expression->getLineNumber());
    }

    void DollarIdentifierReceiverEvaluator::handleIdentifierExpression(Type evaluated,
                                                            Environment & environment,
                                                            IdentifierExpression * expr) const
    {
        auto const identifier = expr->getIdentifierToken().raw;
        if(environment.has(identifier)) {
            auto orig = environment.get(identifier);
            if(orig.m_descriptor == TypeDescriptor::Ints &&
               evaluated.m_descriptor == TypeDescriptor::Int) {

                add<int64_t>(std::move(evaluated), environment, std::move(identifier));

            } else if(orig.m_descriptor == TypeDescriptor::Reals &&
                (evaluated.m_descriptor == TypeDescriptor::Real ||
                 evaluated.m_descriptor == TypeDescriptor::Int)) {

                if(evaluated.m_descriptor == TypeDescriptor::Int) {
                    addIntToRealVector(std::move(evaluated), environment, std::move(identifier));
                } else {
                    add<real>(std::move(evaluated), environment, std::move(identifier));
                }
            } else if(orig.m_descriptor == TypeDescriptor::Bools &&
                evaluated.m_descriptor == TypeDescriptor::Bool) {

                add<bool>(std::move(evaluated), environment, std::move(identifier));

            } else if(orig.m_descriptor == TypeDescriptor::Strings &&
                evaluated.m_descriptor == TypeDescriptor::String) {

                add<std::string>(std::move(evaluated), environment, std::move(identifier));

            } else if(orig.m_descriptor == TypeDescriptor::Bytes &&
                evaluated.m_descriptor == TypeDescriptor::Byte) {

                add<char>(std::move(evaluated), environment, std::move(identifier));

            } else if(orig.m_descriptor == TypeDescriptor::Pods &&
                evaluated.m_descriptor == TypeDescriptor::Pod) {

                add<PodType>(std::move(evaluated), environment, std::move(identifier));

            }  else if(orig.m_descriptor == TypeDescriptor::String &&
                evaluated.m_descriptor == TypeDescriptor::Byte) {

                addToString(std::move(evaluated), environment, std::move(identifier));

            }  else if(orig.m_descriptor == TypeDescriptor::List) {

                addToList(std::move(evaluated), environment, std::move(identifier));

            }
        } else if(evaluated.m_descriptor == TypeDescriptor::Int) {

            add<int64_t>(std::move(evaluated),
                         environment, TypeDescriptor::Ints,
                         std::move(identifier));

        } else if (evaluated.m_descriptor == TypeDescriptor::Real) {

            add<real>(std::move(evaluated),
                             environment, TypeDescriptor::Reals,
                             std::move(identifier));

        } else if (evaluated.m_descriptor == TypeDescriptor::Bool) {

            add<bool>(std::move(evaluated),
                      environment, TypeDescriptor::Bools,
                      std::move(identifier));

        } else if (evaluated.m_descriptor == TypeDescriptor::String) {

            add<std::string>(std::move(evaluated),
                             environment, TypeDescriptor::Strings,
                             std::move(identifier));
        } else if (evaluated.m_descriptor == TypeDescriptor::Byte) {

            add<char>(std::move(evaluated),
                      environment, TypeDescriptor::Bytes,
                      std::move(identifier));
        } else if (evaluated.m_descriptor == TypeDescriptor::Pod) {

            add<PodType>(std::move(evaluated),
                         environment, TypeDescriptor::Pods,
                         std::move(identifier));
        }
    }

}