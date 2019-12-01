#include "QStringExpression.hpp"
#include <utility>

namespace arrow {

    QStringExpression::QStringExpression(long const lineNumber) 
      : Expression(lineNumber)
      , m_qStringToken()
    {
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