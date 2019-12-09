/// (c) Ben Jones 2019 - present

#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>
#include <vector>

namespace arrow {
    
    class StringInputExpression : public Expression
    {
      public:
        StringInputExpression(long const lineNumber);
        StringInputExpression & 
        withExpression(std::shared_ptr<Expression> expression);
        std::shared_ptr<Expression> getExpression() const;
        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;
        std::string toString() const override;
      private:
        std::shared_ptr<Expression> m_expression;
    };

}