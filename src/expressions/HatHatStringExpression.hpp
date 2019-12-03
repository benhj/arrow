#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>


namespace arrow {
    class HatHatStringExpression : public Expression
    {
      public:
        HatHatStringExpression(long const lineNumber);

        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;

        HatHatStringExpression & withHatHatStringToken(Token hatHatStringToken);

        Token getHatHatStringToken() const;

        std::string toString() const override;

      private:
        Token m_hatHatStringToken;
    };
}