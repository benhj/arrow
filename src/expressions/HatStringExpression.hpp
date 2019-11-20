#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>


namespace jasl {
    class HatStringExpression : public Expression
    {
      public:
        HatStringExpression();

        std::shared_ptr<Evaluator> getEvaluator() const override;

        DecayType decayType() const override;

        HatStringExpression & withHatStringToken(Token hatStringToken);

        Token getHatStringToken() const;

        std::string toString() const override;

      private:
        Token m_hatStringToken;
    };
}