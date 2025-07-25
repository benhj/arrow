/// (c) Ben Jones 2019 - present

#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>
#include <string>

namespace arrow {
    
    class LiteralRealExpression final : public Expression
    {
      public:
        LiteralRealExpression(long const lineNumber);

        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;

        LiteralRealExpression & withRealToken(Token realToken);

        Token const & getRealToken() const;

        std::string toString() const override;

      private:
        Token m_realToken;
    };

}