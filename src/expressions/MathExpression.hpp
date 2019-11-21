#pragma once

#include "OperatorExpression.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace jasl {
    
    class MathExpression : public Expression
    {
      public:
        MathExpression();

        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;

        DecayType decayType() const override;

        MathExpression & withLeft(std::shared_ptr<Expression> expressionLeft);
        MathExpression & withRight(std::shared_ptr<Expression> expressionRight);
        MathExpression & withOperator(Token op);
        std::shared_ptr<Expression> getExpressionLeft() const;
        std::shared_ptr<Expression> getExpressionRight() const;
        Token getOperator() const;
        std::string toString() const override;

      private:
        std::shared_ptr<OperatorExpression> m_operatorExp;
    };

}