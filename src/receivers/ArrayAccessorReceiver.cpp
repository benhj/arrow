/// (c) Ben Jones 2019 - present

#include "ArrayAccessorReceiver.hpp"
#include "evaluator/ArrayAccessorReceiverEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

namespace arrow {

    ArrayAccessorReceiver::ArrayAccessorReceiver(long const lineNumber) 
      : Receiver(lineNumber)
      , m_identifier()
    {
    }

    std::shared_ptr<ReceiverEvaluator> 
    ArrayAccessorReceiver::getEvaluator() const
    {
        return std::make_shared<ArrayAccessorReceiverEvaluator>(m_identifier, m_expression);
    }

    ArrayAccessorReceiver & ArrayAccessorReceiver::withIdentifierToken(Token identifier)
    {
        m_identifier = std::move(identifier);
        return *this;
    }
    ArrayAccessorReceiver & ArrayAccessorReceiver::withExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
        return *this;
    }

    Token const & ArrayAccessorReceiver::getIdentifierToken() const
    {
        return m_identifier;
    }
    std::shared_ptr<Expression> ArrayAccessorReceiver::getExpression() const
    {
        return m_expression;
    }

    std::string ArrayAccessorReceiver::toString() const
    {
        return ""; // TODO
    }
}