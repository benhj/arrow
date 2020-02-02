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

    void ArrayAccessorReceiverEvaluator::evaluate(Type incoming, Cache & cache) const
    {
        auto indexEval = m_expression->getEvaluator()->evaluate(cache);

        // Indexing for map creates a new map if it doesn't
        // already exist
        if(indexEval.m_descriptor == TypeDescriptor::String ||
           indexEval.m_descriptor == TypeDescriptor::ListWord) {
            // If item already exists and isn't a map, throw
            std::map<std::string, Type> themap;
            if(cache.has(m_tok)) {
                auto item = cache.get(m_tok);
                if(item.m_descriptor != TypeDescriptor::Map) {
                    throw LanguageException("Incompatible type",
                                            m_expression->getLineNumber());
                }
                themap = std::get<std::map<std::string, Type>>(item.m_variantType);
            }
            auto key = std::get<std::string>(indexEval.m_variantType);
            auto found = themap.find(key);
            if(found != std::end(themap)) {
                found->second = incoming;
            } else {
                themap.emplace(key, incoming);
            }
            cache.add(m_tok, {TypeDescriptor::Map, themap});
            return;
        }

        // For arrays, however, the index is expected to already exist
        if(indexEval.m_descriptor != TypeDescriptor::Int) {
            throw LanguageException("Bad expression for index",
                                    m_expression->getLineNumber());
        }
        auto index = std::get<int64_t>(indexEval.m_variantType);
        try {
            if(!cache.has(m_tok)) {
                throw LanguageException("Array not found",
                                        m_expression->getLineNumber());
            }

            auto item = cache.get(m_tok);
            if(item.m_descriptor == TypeDescriptor::String ||
                item.m_descriptor == TypeDescriptor::ListWord) {
                auto str = std::get<std::string>(item.m_variantType);
                if(incoming.m_descriptor != TypeDescriptor::Byte) {
                    throw LanguageException("Incompatible type",
                        m_expression->getLineNumber());
                }
                auto theChar = std::get<char>(incoming.m_variantType);
                str[index] = theChar;
                cache.add(m_tok, {item.m_descriptor, str});
                return;
            }

            cache.setElementInContainer(m_tok, index, incoming);
        } catch (...) {
            throw LanguageException("Index too big",
                                    m_expression->getLineNumber());
        }
    }
}