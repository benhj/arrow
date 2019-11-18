#pragma once

#include "Expression.hpp"
#include "Token.hpp"
#include <memory>
#include <utility>

namespace jasl {
    class QStringExpression : public Expression
    {
      public:
        QStringExpression()
         : Expression()
         , m_qStringToken()
        {
        }

        QStringExpression & withQStringToken(Token qStringToken)
        {
            m_qStringToken = std::move(qStringToken);
            return *this;
        }

        Token getQStringToken() const
        {
            return m_qStringToken;
        }

        std::string toString() const override 
        {
            std::string str("\n? string expression: ");
            return str.append(m_qStringToken.raw);
        }

      private:
        Token m_qStringToken;
    };
}