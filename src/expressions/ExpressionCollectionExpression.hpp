#pragma once

#include "Expression.hpp"
#include <memory>
#include <vector>

namespace jasl {
    
    /// For storing a set of expressions, e.g.
    /// (a, b, c)
    class ExpressionCollectionExpression : public Expression
    {
      public:
        ExpressionCollectionExpression();

        Type evaluate() const override;

        DecayType decayType() const override;

        void addExpression(std::shared_ptr<Expression> expression);

        std::string toString() const override;

      private:
        std::vector<std::shared_ptr<Expression>> m_expressions;
    };

}