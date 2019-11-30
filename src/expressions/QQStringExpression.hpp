#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace arrow {
    class QQStringExpression : public Expression
    {
      public:
        QQStringExpression(long const lineNumber);

        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;

        DecayType decayType() const override;

        QQStringExpression & withQQStringToken(Token qQStringToken);

        Token getQQStringToken() const;

        std::string toString() const override;

      private:
        Token m_qQStringToken;
    };
}