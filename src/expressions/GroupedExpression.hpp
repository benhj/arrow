/// (c) Ben Jones 2019 - present

#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace arrow {
    
    class GroupedExpression final : public Expression
    {
      public:
        GroupedExpression(long const lineNumber);

        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;

        GroupedExpression & withExpression(std::shared_ptr<Expression> expression);
        
        std::shared_ptr<Expression> const & getExpression() const;
        std::string toString() const override;

      private:
        std::shared_ptr<Expression> m_expression;
    };

}