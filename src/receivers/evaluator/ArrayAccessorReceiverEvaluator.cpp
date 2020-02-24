/// (c) Ben Jones 2019

#include "ArrayAccessorReceiverEvaluator.hpp"
#include "expressions/IdentifierExpression.hpp"
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

        auto const & environmentKey = m_tok.raw;

        // Pod 'indexing' -- plain member word
        if(environment.has(environmentKey)) {
            auto item = environment.findAndRetrieveCached(environmentKey);
            // If item already exists and isn't a map, throw
            if(item->second.m_descriptor == TypeDescriptor::Pod) {
                auto & pod = std::get<PodType>(item->second.m_variantType);
                auto & data = pod.m_namedData;
                auto ie = dynamic_cast<IdentifierExpression*>(m_expression.get());
                if(!ie) {
                    throw LanguageException("Bad pod accessor", m_expression->getLineNumber());
                }
                auto key = ie->getIdentifierToken().raw;
                auto found = data.find(key);
                if(found != std::end(data)) {
                    found->second = incoming;
                    return;
                } else {
                    throw LanguageException("Unknown pod member", m_expression->getLineNumber());
                }
            }
        }

        // For indexing of all other container types,
        // the index key needs to be evaluated.
        auto indexEval = m_expression->getEvaluator()->evaluate(environment);

        // Check if item exists (note, a map which is keyed on a string
        // or list word is allowed to not exist).
        if(!environment.has(environmentKey) &&
           (indexEval.m_descriptor != TypeDescriptor::String &&
            indexEval.m_descriptor  != TypeDescriptor::ListWord)) {
            throw LanguageException("Type not found", m_expression->getLineNumber());
        }

        // Indexing for map is always keyed on a string and
        // creates a new map if it doesn't already exist
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