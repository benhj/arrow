#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace arrow {
    
    class IdentifierExpression : public Expression
    {
      public:
        IdentifierExpression();

        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;

        DecayType decayType() const override;

        IdentifierExpression & withIdentifierToken(Token identifier);

        Token getIdentifierToken() const;

        std::string toString() const override;

      private:
        Token m_identifier;
    };

}