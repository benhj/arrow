#include "QStringExpression.hpp"
#include <utility>

namespace jasl {

    QStringExpression::QStringExpression()
     : Expression()
     , m_qStringToken()
    {
    }

    Type QStringExpression::evaluate() const
    {
        return {TypeDescriptor::None, {false}};
    }

    DecayType QStringExpression::decayType() const
    {
        return DecayType::DECAYS_TO_STRING;
    }

    QStringExpression & QStringExpression::withQStringToken(Token qStringToken)
    {
        m_qStringToken = std::move(qStringToken);
        return *this;
    }

    Token QStringExpression::getQStringToken() const
    {
        return m_qStringToken;
    }

    std::string QStringExpression::toString() const
    {
        std::string str("\n? string expression: ");
        return str.append(m_qStringToken.raw);
    }
}