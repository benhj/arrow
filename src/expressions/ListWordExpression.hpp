#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace arrow {
    
    class ListWordExpression : public Expression
    {
      public:
        ListWordExpression(long const lineNumber);

        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;

        ListWordExpression & withWordToken(Token identifier);

        Token getWordToken() const;

        std::string toString() const override;

      private:
        Token m_word;
    };

}