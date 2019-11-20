#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace jasl {
    
    class OperatorExpression : public Expression
    {
      public:
        OperatorExpression();

        std::shared_ptr<Evaluator> getEvaluator() const override;

        Type evaluate() const override;

        DecayType decayType() const override;

        OperatorExpression & withLeft(std::shared_ptr<Expression> expressionLeft);
        OperatorExpression & withRight(std::shared_ptr<Expression> expressionRight);
        OperatorExpression & withOperator(Token op);
        std::shared_ptr<Expression> getExpressionLeft() const;
        std::shared_ptr<Expression> getExpressionRight() const;
        Token getOperator() const;
        std::string toString() const override;

      private:
        std::shared_ptr<Expression> m_expressionLeft;
        Token m_op;
        std::shared_ptr<Expression> m_expressionRight;
    };

}