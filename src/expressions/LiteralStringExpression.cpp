#include "LiteralStringExpression.hpp"
#include <utility>

namespace jasl {

    LiteralStringExpression::LiteralStringExpression()
     : Expression()
     , m_stringToken()
    {
    }

    std::shared_ptr<Evaluator> LiteralStringExpression::getEvaluator() const
    {
        return nullptr;
    }

    Type LiteralStringExpression::evaluate() const
    {
        return {TypeDescriptor::String, m_stringToken.raw};
    }

    DecayType LiteralStringExpression::decayType() const
    {
        return DecayType::DECAYS_TO_STRING;
    }

    LiteralStringExpression & LiteralStringExpression::withStringToken(Token stringToken)
    {
        m_stringToken = std::move(stringToken);
        return *this;
    }

    Token LiteralStringExpression::getStringToken() const
    {
        return m_stringToken;
    }

    std::string LiteralStringExpression::toString() const
    {
        std::string str("\nLiteral string expression: ");
        return str.append(m_stringToken.raw);
    }
}