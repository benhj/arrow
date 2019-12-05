/// (c) Ben Jones 2019 - present

#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>
#include <string>

namespace arrow {
    
    class LiteralRealExpression : public Expression
    {
      public:
        LiteralRealExpression(long const lineNumber);

        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;

        LiteralRealExpression & withRealToken(Token realToken);

        Token getRealToken() const;

        std::string toString() const override;

      private:
        Token m_realToken;
    };

}