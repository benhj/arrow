#include "LiteralIntExpression.hpp"
#include <utility>

namespace jasl {

    LiteralIntExpression::LiteralIntExpression()
     : Expression()
     , m_intToken()
    {
    }

    std::shared_ptr<Evaluator> LiteralIntExpression::getEvaluator() const
    {
        return nullptr;
    }
/*
    Type LiteralIntExpression::evaluate() const
    {
        return {TypeDescriptor::Int, {std::stoll(m_intToken.raw)}};
    }
*/
    DecayType LiteralIntExpression::decayType() const
    {
        return DecayType::DECAYS_TO_INT;
    }

    LiteralIntExpression & LiteralIntExpression::withIntToken(Token intToken)
    {
        m_intToken = std::move(intToken);
        return *this;
    }

    Token LiteralIntExpression::getIntToken() const
    {
        return m_intToken;
    }

    std::string LiteralIntExpression::toString() const
    {
        std::string str("\nLiteral int expression: ");
        return str.append(m_intToken.raw);
    }
}