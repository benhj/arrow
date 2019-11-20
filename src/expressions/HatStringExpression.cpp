#include "HatStringExpression.hpp"
#include <utility>

namespace jasl {

    HatStringExpression::HatStringExpression()
     : Expression()
     , m_hatStringToken()
    {
    }

    std::shared_ptr<Evaluator> HatStringExpression::getEvaluator() const
    {
        return nullptr;
    }
/*
    Type HatStringExpression::evaluate() const
    {
        return {TypeDescriptor::None, {false}};
    }
*/
    DecayType HatStringExpression::decayType() const
    {
        return DecayType::DECAYS_TO_STRING;
    }

    HatStringExpression & HatStringExpression::withHatStringToken(Token hatStringToken)
    {
        m_hatStringToken = std::move(hatStringToken);
        return *this;
    }

    Token HatStringExpression::getHatStringToken() const
    {
        return m_hatStringToken;
    }

    std::string HatStringExpression::toString() const
    {
        std::string str("\n^ string expression: ");
        return str.append(m_hatStringToken.raw);
    }
}