#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace jasl {
    class QQStringExpression : public Expression
    {
      public:
        QQStringExpression();

        std::shared_ptr<Evaluator> getEvaluator() const override;

        DecayType decayType() const override;

        QQStringExpression & withQQStringToken(Token qQStringToken);

        Token getQQStringToken() const;

        std::string toString() const override;

      private:
        Token m_qQStringToken;
    };
}