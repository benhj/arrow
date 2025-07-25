/// (c) Ben Jones 2019 - present

#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace arrow {
    
    class LiteralStringExpression final : public Expression
    {
      public:
        LiteralStringExpression(long const lineNumber);

        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;

        LiteralStringExpression & withStringToken(Token stringToken);

        Token const & getStringToken() const;

        std::string toString() const override;

      private:
        Token m_stringToken;
    };

}