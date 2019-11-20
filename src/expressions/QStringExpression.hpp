#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace jasl {
    class QStringExpression : public Expression
    {
      public:
        QStringExpression();

        std::shared_ptr<Evaluator> getEvaluator() const override;

        DecayType decayType() const override;

        QStringExpression & withQStringToken(Token qStringToken);

        Token getQStringToken() const;

        std::string toString() const override;

      private:
        Token m_qStringToken;
    };
}