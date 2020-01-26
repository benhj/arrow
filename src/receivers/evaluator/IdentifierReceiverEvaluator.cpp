/// (c) Ben Jones 2019

#include "IdentifierReceiverEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

namespace arrow {

    IdentifierReceiverEvaluator::IdentifierReceiverEvaluator(Token tok) 
      : m_tok(std::move(tok))
    {
    }

    void IdentifierReceiverEvaluator::evaluate(Type incoming, Cache & cache) const
    {
        // automatically does a replace
        cache.add(m_tok, incoming);
    }
}