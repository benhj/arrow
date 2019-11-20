#include "LiteralRealExpression.hpp"
#include <utility>

namespace jasl {

    LiteralRealExpression::LiteralRealExpression()
     : Expression()
     , m_realToken()
    {
    }

    std::shared_ptr<Evaluator> LiteralRealExpression::getEvaluator() const
    {
        return nullptr;
    }

    Type LiteralRealExpression::evaluate() const
    {
        return {TypeDescriptor::Real, {std::stold(m_realToken.raw)}};
    }

    DecayType LiteralRealExpression::decayType() const
    {
        return DecayType::DECAYS_TO_REAL;
    }

    LiteralRealExpression & LiteralRealExpression::withRealToken(Token realToken)
    {
        m_realToken = std::move(realToken);
        return *this;
    }

    Token LiteralRealExpression::getRealToken() const
    {
        return m_realToken;
    }

    std::string LiteralRealExpression::toString() const
    {
        std::string str("\nLiteral real expression: ");
        return str.append(m_realToken.raw);
    }
}