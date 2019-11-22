#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace jasl {
    
    class ListWordExpression : public Expression
    {
      public:
        ListWordExpression();

        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;

        DecayType decayType() const override;

        ListWordExpression & withWordToken(Token identifier);

        Token getWordToken() const;

        std::string toString() const override;

      private:
        Token m_word;
    };

}