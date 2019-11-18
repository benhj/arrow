#pragma once

#include "Expression.hpp"
#include "Token.hpp"
#include <memory>
#include <utility>

namespace jasl {
    class QQStringExpression : public Expression
    {
      public:
        QQStringExpression()
         : Expression()
         , m_qQStringToken()
        {
        }

        QQStringExpression & withQQStringToken(Token qQStringToken)
        {
            m_qQStringToken = std::move(qQStringToken);
            return *this;
        }

        Token getQQStringToken() const
        {
            return m_qQStringToken;
        }

        std::string toString() const override 
        {
            std::string str("\n?? string expression: ");
            return str.append(m_qQStringToken.raw);
        }

      private:
        Token m_qQStringToken;
    };
}