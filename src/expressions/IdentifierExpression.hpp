/// (c) Ben Jones 2019 - present

#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace arrow {
    
    class IdentifierExpression final : public Expression
    {
      public:
        IdentifierExpression(long const lineNumber);

        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;

        IdentifierExpression & withIdentifierToken(Token identifier);

        Token const & getIdentifierToken() const;

        std::string toString() const override;

        char const * getTypeString() const override;

      private:
        Token m_identifier;
    };

}