/// (c) Ben Jones 2019 - present

#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace arrow {
    
    class LiteralIntExpression final : public Expression
    {
      public:
        LiteralIntExpression(long const lineNumber);

        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;

        LiteralIntExpression & withIntToken(Token intToken);

        Token const & getIntToken() const;

        std::string toString() const override;

      private:
        Token m_intToken;
    };

}