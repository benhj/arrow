/// (c) Ben Jones 2019 - present

#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>


namespace arrow {
    class HatStringExpression : public Expression
    {
      public:
        HatStringExpression(long const lineNumber);

        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;

        HatStringExpression & withHatStringToken(Token hatStringToken);

        Token const & getHatStringToken() const;

        std::string toString() const override;

      private:
        Token m_hatStringToken;
    };
}