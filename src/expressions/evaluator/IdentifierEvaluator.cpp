/// (c) Ben Jones 2019

#include "IdentifierEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

namespace arrow {

    IdentifierEvaluator::IdentifierEvaluator(Token tok) 
      : m_tok(std::move(tok))
    {
    }

    Type IdentifierEvaluator::evaluate(Environment & environment) const
    {
        auto const & environmentKey = m_tok.raw;
        if(!environment.has(environmentKey)) {
            throw LanguageException("Identifier not found",
                                    m_tok.lineNumber);
        }
        return environment.get(environmentKey);
    }
}