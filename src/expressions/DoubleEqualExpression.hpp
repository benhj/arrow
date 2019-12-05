/// (c) Ben Jones 2019 - present

#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>


namespace arrow {
    class DoubleEqualExpression : public Expression
    {
      public:
        DoubleEqualExpression(long const lineNumber);
        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;
        DoubleEqualExpression & withToken(Token token);
        std::string toString() const override;

      private:
        Token m_token;
    };
}