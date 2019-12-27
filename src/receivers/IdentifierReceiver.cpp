/// (c) Ben Jones 2019 - present

#include "IdentifierReceiver.hpp"
#include "evaluator/receivers/ReceiverEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

namespace arrow {

    IdentifierReceiver::IdentifierReceiver(long const lineNumber) 
      : Receiver(lineNumber)
      , m_identifier()
    {
    }

    std::shared_ptr<ReceiverEvaluator> 
    IdentifierReceiver::getEvaluator() const
    {
        /*
        struct IdentifierEvaluator : public ReceiverEvaluator {
            IdentifierEvaluator(Token tok) : m_tok(std::move(tok))
            {
            }

            Type evaluate(Cache & cache) const override
            {
                if(m_tok.raw == "true") {
                    return {TypeDescriptor::Bool, true};
                } else if(m_tok.raw == "false") {
                    return {TypeDescriptor::Bool, false};
                } else if(!cache.has(m_tok)) {
                    throw LanguageException("Value for identifier not found", m_tok.lineNumber);
                }
                return cache.get(m_tok);
            }
          private:
            Token m_tok;
        };
        return std::make_shared<IdentifierEvaluator>(m_identifier);
        */
        return nullptr;
    }

    IdentifierReceiver & IdentifierReceiver::withIdentifierToken(Token identifier)
    {
        m_identifier = std::move(identifier);
        return *this;
    }

    Token IdentifierReceiver::getIdentifierToken() const
    {
        return  m_identifier;
    }

    std::string IdentifierReceiver::toString() const
    {
        std::string str("\nIdentifier expression: ");
        return str.append(m_identifier.raw);
    }
}