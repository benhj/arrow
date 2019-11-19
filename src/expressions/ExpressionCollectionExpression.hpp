#pragma once

#include "Expression.hpp"
#include <memory>
#include <utility>
#include <vector>

namespace jasl {
    
    /// For storing a set of expressions, e.g.
    /// (a, b, c)
    class ExpressionCollectionExpression : public Expression
    {
      public:
        ExpressionCollectionExpression()
         : Expression()
        {
        }

        DecayType decayType() const override
        {
            return DecayType::DECAYS_TO_NONE;
        }

        void addExpression(std::shared_ptr<Expression> expression)
        {
            m_expressions.emplace_back(std::move(expression));
        }

        std::string toString() const override 
        {
            if(!m_expressions.empty()) {
                std::string str("\nBegin expression collection: ");
                for(auto const & exp : m_expressions) {
                    str.append(exp->toString());
                }
                str.append("\nEnd expression collection.");
                return str;
            }
            return "";
        }

      private:
        std::vector<std::shared_ptr<Expression>> m_expressions;
    };

}