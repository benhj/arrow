#pragma once

#include "OperatorExpression.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace jasl {
    
    class MathExpression : public Expression
    {
      public:
        MathExpression() 
        : Expression()
        , m_operatorExp(std::make_shared<OperatorExpression>())
        {
        }

        Type evaluate() const override
        {
            return {TypeDescriptor::Real, {false}};
        }

        DecayType decayType() const override
        {
            return DecayType::DECAYS_TO_REAL;
        }

        MathExpression & withLeft(std::shared_ptr<Expression> expressionLeft)
        {
            m_operatorExp->withLeft(std::move(expressionLeft));
            return *this;
        }
        MathExpression & withRight(std::shared_ptr<Expression> expressionRight)
        {
            m_operatorExp->withRight(std::move(expressionRight));
            return *this;
        }
        MathExpression & withOperator(Token op)
        {
            m_operatorExp->withOperator(std::move(op));
            return *this;
        }
        std::shared_ptr<Expression> getExpressionLeft() const
        {
            return m_operatorExp->getExpressionLeft();
        }
        std::shared_ptr<Expression> getExpressionRight() const
        {
            return m_operatorExp->getExpressionRight();
        }
        Token getOperator() const
        {
            return m_operatorExp->getOperator();
        }
        std::string toString() const override 
        {
            return m_operatorExp->toString();
        }

      private:
        std::shared_ptr<OperatorExpression> m_operatorExp;
    };

}