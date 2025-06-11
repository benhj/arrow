/// (c) Ben Jones 2020 - present

#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace arrow {
    
    class NegativeNumberExpression final : public Expression
    {
      public:
        NegativeNumberExpression(long const lineNumber);

        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;

        NegativeNumberExpression & withExpression(std::shared_ptr<Expression> expression);
        
        std::shared_ptr<Expression> const & getExpression() const;
        std::string toString() const override;

      private:
        std::shared_ptr<Expression> m_expression;
    };

}