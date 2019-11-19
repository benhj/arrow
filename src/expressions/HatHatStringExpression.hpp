#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>
#include <utility>

namespace jasl {
    class HatHatStringExpression : public Expression
    {
      public:
        HatHatStringExpression()
         : Expression()
         , m_hatHatStringToken()
        {
        }

        DecayType decayType() const override
        {
            return DecayType::DECAYS_TO_STRING;
        }

        HatHatStringExpression & withHatHatStringToken(Token hatHatStringToken)
        {
            m_hatHatStringToken = std::move(hatHatStringToken);
            return *this;
        }

        Token getHatHatStringToken() const
        {
            return m_hatHatStringToken;
        }

        std::string toString() const override 
        {
            std::string str("\n^^ string expression: ");
            return str.append(m_hatHatStringToken.raw);
        }

      private:
        Token m_hatHatStringToken;
    };
}