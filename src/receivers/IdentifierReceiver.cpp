/// (c) Ben Jones 2019 - present

#include "IdentifierReceiver.hpp"
#include "evaluator/receivers/IdentifierReceiverEvaluator.hpp"
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
        return std::make_shared<IdentifierReceiverEvaluator>(m_identifier);
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