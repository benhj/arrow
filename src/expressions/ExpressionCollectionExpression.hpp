/// (c) Ben Jones 2019 - present

#pragma once

#include "Expression.hpp"
#include <memory>
#include <vector>

namespace arrow {
    
    /// For storing a set of expressions, e.g.
    /// (a, b, c)
    class ExpressionCollectionExpression final : public Expression
    {
      public:
        ExpressionCollectionExpression(long const lineNumber);

        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;

        void addExpression(std::shared_ptr<Expression> expression);
        std::vector<std::shared_ptr<Expression>> const & getExpressionCollection() const;

        std::string toString() const override;

      private:
        std::vector<std::shared_ptr<Expression>> m_expressions;
    };

}