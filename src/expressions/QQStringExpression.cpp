#include "QQStringExpression.hpp"
#include <utility>

namespace arrow {

    QQStringExpression::QQStringExpression(long const lineNumber) 
      : Expression(lineNumber)
      , m_qQStringToken()
    {
    }

    DecayType QQStringExpression::decayType() const
    {
        return DecayType::DECAYS_TO_STRING;
    }

    QQStringExpression & QQStringExpression::withQQStringToken(Token qQStringToken)
    {
        m_qQStringToken = std::move(qQStringToken);
        return *this;
    }

    Token QQStringExpression::getQQStringToken() const
    {
        return m_qQStringToken;
    }

    std::string QQStringExpression::toString() const
    {
        std::string str("\n?? string expression: ");
        return str.append(m_qQStringToken.raw);
    }
}