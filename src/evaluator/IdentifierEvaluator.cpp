#include "IdentifierEvaluator.hpp"
#include <utility>

namespace jasl {

    IdentifierEvaluator::IdentifierEvaluator(Token tok) 
      : m_tok(std::move(tok))
    {
    }

    Type IdentifierEvaluator::evaluate(Cache & cache) const
    {
        if(!cache.has(m_tok.raw)) {
            throw std::runtime_error("Value for identifier not found.");
        }
        return cache.get(m_tok.raw);
    }
}