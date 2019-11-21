#include "HatHatStringExpression.hpp"
#include <utility>

namespace jasl {

    HatHatStringExpression::HatHatStringExpression()
     : Expression()
     , m_hatHatStringToken()
    {
    }

    std::shared_ptr<ExpressionEvaluator> HatHatStringExpression::getEvaluator() const
    {
        return nullptr;
    }
/*
    Type HatHatStringExpression::evaluate() const
    {
        return {TypeDescriptor::None, {false}};
    }
*/
    DecayType HatHatStringExpression::decayType() const
    {
        return DecayType::DECAYS_TO_STRING;
    }

    HatHatStringExpression & HatHatStringExpression::withHatHatStringToken(Token hatHatStringToken)
    {
        m_hatHatStringToken = std::move(hatHatStringToken);
        return *this;
    }

    Token HatHatStringExpression::getHatHatStringToken() const
    {
        return m_hatHatStringToken;
    }

    std::string HatHatStringExpression::toString() const
    {
        std::string str("\n^^ string expression: ");
        return str.append(m_hatHatStringToken.raw);
    }
}