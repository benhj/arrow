/// (c) Ben Jones 2019

#include "ArrayAccessorReceiverEvaluator.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

namespace arrow {

    ArrayAccessorReceiverEvaluator::ArrayAccessorReceiverEvaluator(Token tok, std::shared_ptr<Expression> expression) 
      : m_tok(std::move(tok))
      , m_expression(std::move(expression))
    {
    }

    void ArrayAccessorReceiverEvaluator::evaluate(Type incoming, Environment & cache) const
    {
        auto indexEval = m_expression->getEvaluator()->evaluate(cache);
        auto const & cacheKey = m_tok.raw;

        // Indexing for map creates a new map if it doesn't
        // already exist
        if(indexEval.m_descriptor == TypeDescriptor::String ||
           indexEval.m_descriptor == TypeDescriptor::ListWord) {

            auto mapKey = std::get<std::string>(indexEval.m_variantType);
            // When already exists, insert by reference
            if(cache.has(cacheKey)) {
                auto item = cache.findAndRetrieveEnvironmentd(cacheKey);
                // If item already exists and isn't a map, throw
                if(item->second.m_descriptor != TypeDescriptor::Map) {
                    throw LanguageException("Incompatible type",
                                            m_expression->getLineNumber());
                }
                auto & themap = std::get<std::map<std::string, Type>>(item->second.m_variantType);
                auto found = themap.find(mapKey);
                if(found != std::end(themap)) {
                    found->second = incoming;
                } else {
                    themap.emplace(mapKey, incoming);
                }
                return;
            }
            // Else create a brand new map with initial element
            std::map<std::string, Type> themap;
            themap.emplace(mapKey, incoming);
            cache.add(cacheKey, {TypeDescriptor::Map, themap});
            return;
        }

        // For arrays, however, the index is expected to already exist
        if(indexEval.m_descriptor != TypeDescriptor::Int) {
            throw LanguageException("Bad expression for index",
                                    m_expression->getLineNumber());
        }
        auto index = std::get<int64_t>(indexEval.m_variantType);
        try {
            if(!cache.has(cacheKey)) {
                throw LanguageException("Array not found",
                                        m_expression->getLineNumber());
            }

            // Assumed to exist given above check
            auto item = cache.findAndRetrieveEnvironmentd(cacheKey);
            if(item->second.m_descriptor == TypeDescriptor::String ||
                item->second.m_descriptor == TypeDescriptor::ListWord) {
                auto & str = std::get<std::string>(item->second.m_variantType);
                if(incoming.m_descriptor != TypeDescriptor::Byte) {
                    throw LanguageException("Incompatible type",
                        m_expression->getLineNumber());
                }
                auto theChar = std::get<char>(incoming.m_variantType);
                str[index] = theChar;
                return;
            }

            cache.setElementInContainer(cacheKey, index, incoming);
        } catch (...) {
            throw LanguageException("Index too big",
                                    m_expression->getLineNumber());
        }
    }
}