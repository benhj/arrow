#include "IdentifierExpression.hpp"
#include <utility>

namespace jasl {

    IdentifierExpression::IdentifierExpression()
     : Expression()
     , m_identifier()
    {
    }

    std::shared_ptr<Evaluator> IdentifierExpression::getEvaluator() const
    {
        return nullptr;
    }

    Type IdentifierExpression::evaluate() const
    {
        return {TypeDescriptor::None, {false}};
    }

    DecayType IdentifierExpression::decayType() const
    {
        return DecayType::DECAYS_TO_STRING;
    }

    IdentifierExpression & IdentifierExpression::withIntToken(Token identifier)
    {
        m_identifier = std::move(identifier);
        return *this;
    }

    Token IdentifierExpression::getIntToken() const
    {
        return  m_identifier;
    }

    std::string IdentifierExpression::toString() const
    {
        std::string str("\nIdentifier expression: ");
        return str.append(m_identifier.raw);
    }
}