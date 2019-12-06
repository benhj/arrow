/// (c) Ben Jones 2019 - present

#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace arrow {
    
    class LiteralCharExpression : public Expression
    {
      public:
        LiteralCharExpression(long const lineNumber);

        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;

        LiteralCharExpression & withStringToken(Token stringToken);

        Token getStringToken() const;

        std::string toString() const override;

      private:
        Token m_stringToken;
    };

}