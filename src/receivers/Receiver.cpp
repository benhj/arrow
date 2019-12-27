/// (c) Ben Jones 2019 - present

#include "Receiver.hpp"
#include "parser/LanguageException.hpp"

namespace arrow {
    Receiver::Receiver(long const lineNumber)
      : Node()
      , m_lineNumber(lineNumber)
    {
    }

    std::shared_ptr<ReceiverEvaluator> Receiver::getEvaluator() const
    {
        throw LanguageException("Cannot evaluate expression", m_lineNumber);
    }

    long Receiver::getLineNumber() const
    {
        return m_lineNumber;
    }
}