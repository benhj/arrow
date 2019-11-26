#include "IdentifierEvaluator.hpp"
#include <utility>

namespace arrow {

    IdentifierEvaluator::IdentifierEvaluator(Token tok) 
      : m_tok(std::move(tok))
    {
    }

    Type IdentifierEvaluator::evaluate(Cache & cache) const
    {
        if(!cache.has(m_tok)) {
            std::string error("Indentifier not found on line ");
            error.append(std::to_string(m_tok.lineNumber));
            throw std::runtime_error(error);
        }
        return cache.get(m_tok);
    }
}