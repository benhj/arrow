/// (c) Ben Jones 2019

#include "ArrayAccessorReceiverEvaluator.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
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
            cache.setElementInContainer(m_tok, index, incoming);
        } catch (...) {
            throw LanguageException("Index too big",
                                    m_expression->getLineNumber());
        }
    }
}