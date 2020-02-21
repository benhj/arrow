/// (c) Ben Jones 2019

#include "IdentifierEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

namespace arrow {

    IdentifierEvaluator::IdentifierEvaluator(Token tok) 
      : m_tok(std::move(tok))
    {
    }

    Type IdentifierEvaluator::evaluate(Environment & cache) const
    {
        auto const & cacheKey = m_tok.raw;
        if(!cache.has(cacheKey)) {
            throw LanguageException("Identifier not found",
                                    m_tok.lineNumber);
        }
        return cache.get(cacheKey);
    }
}