#pragma once

#include "Expression.hpp"
#include "Token.hpp"
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
            std::string str("^^ string expression: ");
            return str.append(m_hatHatStringToken.raw);
        }

      private:
        Token m_hatHatStringToken;
    };
}