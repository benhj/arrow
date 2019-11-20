#include "QQStringExpression.hpp"
#include <utility>

namespace jasl {

    QQStringExpression::QQStringExpression()
     : Expression()
     , m_qQStringToken()
    {
    }

    std::shared_ptr<Evaluator> QQStringExpression::getEvaluator() const
    {
        return nullptr;
    }

    Type QQStringExpression::evaluate() const
    {
        return {TypeDescriptor::None, {false}};
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