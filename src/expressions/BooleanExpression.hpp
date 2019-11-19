#pragma once

#include "OperatorExpression.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace jasl {
    
    class BooleanExpression : public Expression
    {
      public:
        BooleanExpression() 
        : Expression()
        , m_operatorExp(std::make_shared<OperatorExpression>())
        {
        }

        DecayType decayType() const override
        {
            return DecayType::DECAYS_TO_BOOL;
        }

        BooleanExpression & withLeft(std::shared_ptr<Expression> expressionLeft)
        {
            m_operatorExp->withLeft(std::move(expressionLeft));
            return *this;
        }
        BooleanExpression & withRight(std::shared_ptr<Expression> expressionRight)
        {
            m_operatorExp->withRight(std::move(expressionRight));
            return *this;
        }
        BooleanExpression & withOperator(Token op)
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