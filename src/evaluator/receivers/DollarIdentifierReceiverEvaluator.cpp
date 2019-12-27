/// (c) Ben Jones 2019

#include "DollarIdentifierReceiverEvaluator.hpp"
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

    DollarIdentifierReceiverEvaluator::DollarIdentifierReceiverEvaluator(Token tok) 
      : m_tok(std::move(tok))
    {
    }

    void DollarIdentifierReceiverEvaluator::evaluate(Type evaluated, Cache & cache) const
    {
        auto identifier = m_tok;
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