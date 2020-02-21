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

    void ArrayAccessorReceiverEvaluator::evaluate(Type incoming, Environment & environment) const
    {
        auto indexEval = m_expression->getEvaluator()->evaluate(environment);
        auto const & environmentKey = m_tok.raw;

        // Indexing for map creates a new map if it doesn't
        // already exist
        if(indexEval.m_descriptor == TypeDescriptor::String ||
           indexEval.m_descriptor == TypeDescriptor::ListWord) {

            auto mapKey = std::get<std::string>(indexEval.m_variantType);
            // When already exists, insert by reference
            if(environment.has(environmentKey)) {
                auto item = environment.findAndRetrieveCached(environmentKey);
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
            environment.add(environmentKey, {TypeDescriptor::Map, themap});
            return;
        }

        // For arrays, however, the index is expected to already exist
        if(indexEval.m_descriptor != TypeDescriptor::Int) {
            throw LanguageException("Bad expression for index",
                                    m_expression->getLineNumber());
        }
        auto index = std::get<int64_t>(indexEval.m_variantType);
        try {
            if(!environment.has(environmentKey)) {
                throw LanguageException("Array not found",
                                        m_expression->getLineNumber());
            }

            // Assumed to exist given above check
            auto item = environment.findAndRetrieveCached(environmentKey);
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

            environment.setElementInContainer(environmentKey, index, incoming);
        } catch (...) {
            throw LanguageException("Index too big",
                                    m_expression->getLineNumber());
        }
    }
}