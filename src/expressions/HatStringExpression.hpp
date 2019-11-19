#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>
#include <utility>

namespace jasl {
    class HatStringExpression : public Expression
    {
      public:
        HatStringExpression()
         : Expression()
         , m_hatStringToken()
        {
        }

        HatStringExpression & withHatStringToken(Token hatStringToken)
        {
            m_hatStringToken = std::move(hatStringToken);
            return *this;
        }

        Token getHatStringToken() const
        {
            return m_hatStringToken;
        }

        std::string toString() const override 
        {
            std::string str("\n^ string expression: ");
            return str.append(m_hatStringToken.raw);
        }

      private:
        Token m_hatStringToken;
    };
}