#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace arrow {
    
    class OperatorExpression : public Expression
    {
      public:
        OperatorExpression(long const lineNumber);

        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;

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