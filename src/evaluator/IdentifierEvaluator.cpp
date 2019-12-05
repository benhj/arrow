/// (c) Ben Jones 2019

#include "IdentifierEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

namespace arrow {

    IdentifierEvaluator::IdentifierEvaluator(Token tok) 
      : m_tok(std::move(tok))
    {
    }

    Type IdentifierEvaluator::evaluate(Cache & cache) const
    {
        if(!cache.has(m_tok)) {
            throw LanguageException("Identifier not found",
                                    m_tok.lineNumber);
        }
        return cache.get(m_tok);
    }
}