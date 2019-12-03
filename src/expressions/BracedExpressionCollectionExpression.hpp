#pragma once

#include "Expression.hpp"
#include <memory>
#include <vector>

namespace arrow {
    
    /// For storing a set of expressions, e.g.
    /// {a, b, c}
    class BracedExpressionCollectionExpression : public Expression
    {
      public:
        BracedExpressionCollectionExpression(long const lineNumber);

        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;
        void addExpression(std::shared_ptr<Expression> expression);
        std::vector<std::shared_ptr<Expression>> getExpressionCollection() const;

        std::string toString() const override;

      private:
        std::vector<std::shared_ptr<Expression>> m_expressions;
    };

}