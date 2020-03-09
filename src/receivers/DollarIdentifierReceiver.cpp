/// (c) Ben Jones 2019 - present

#include "DollarIdentifierReceiver.hpp"
#include "evaluator/DollarIdentifierReceiverEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

namespace arrow {

    DollarIdentifierReceiver::DollarIdentifierReceiver(long const lineNumber) 
      : Receiver(lineNumber)
      , m_identifier()
    {
    }

    std::shared_ptr<ReceiverEvaluator> 
    DollarIdentifierReceiver::getEvaluator() const
    {
        return std::make_shared<DollarIdentifierReceiverEvaluator>(m_identifier);
    }

    DollarIdentifierReceiver & DollarIdentifierReceiver::withIdentifierToken(Token identifier)
    {
        m_identifier = std::move(identifier);
        return *this;
    }

    Token const & DollarIdentifierReceiver::getIdentifierToken() const
    {
        return m_identifier;
    }

    std::string DollarIdentifierReceiver::toString() const
    {
        std::string str("\nIdentifier expression: ");
        return str.append(m_identifier.raw);
    }
}