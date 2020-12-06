/// (c) Ben Jones 2019 - present

#include "DollarIdentifierReceiver.hpp"
#include "evaluator/DollarIdentifierReceiverEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

namespace arrow {

    DollarIdentifierReceiver::DollarIdentifierReceiver(long const lineNumber) 
      : Receiver(lineNumber)
    {
    }

    std::shared_ptr<ReceiverEvaluator> 
    DollarIdentifierReceiver::getEvaluator() const
    {
        return std::make_shared<DollarIdentifierReceiverEvaluator>(m_expression);
    }

    DollarIdentifierReceiver & DollarIdentifierReceiver::withExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
        return *this;
    }

    std::shared_ptr<Expression> const & DollarIdentifierReceiver::getExpression() const
    {
        return m_expression;
    }

    std::string DollarIdentifierReceiver::toString() const
    {
        return ""; // TODO
    }
}