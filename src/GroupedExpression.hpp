#pragma once

#include "Expression.hpp"
#include "Token.hpp"
#include <memory>

namespace jasl {
    
    class GroupedExpression : public Expression
    {
      public:
        GroupedExpression() 
        : Expression()
        , m_expression{nullptr}
        {
        }

        GroupedExpression & withExpression(std::shared_ptr<Expression> expression)
        {
            m_expression = std::move(expression);
            return *this;
        }
        
        std::shared_ptr<Expression> getExpression() const
        {
            return m_expression;
        }
        std::string toString() const override 
        {
            std::string str("\nBegin grouped expression");
            str.append(m_expression->toString());
            str.append("\nEnd grouped expression");
            return str;
        }

      private:
        std::shared_ptr<Expression> m_expression;
    };

}