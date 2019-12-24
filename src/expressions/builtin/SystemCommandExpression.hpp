/// (c) Ben Jones 2019 - present

#pragma once

#include "BuiltInFunctionExpression.hpp"
#include "expressions/Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>
#include <vector>

namespace arrow {
    
    class SystemCommandExpression : public Expression, public BuiltInFunctionExpression
    {
      public:
        SystemCommandExpression(long const lineNumber);
        SystemCommandExpression & 
        withExpression(std::shared_ptr<Expression> expression);
        std::shared_ptr<Expression> getExpression() const;
        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;
        std::string toString() const override;
        std::string getName() const override;
      private:
        std::shared_ptr<Expression> m_expression;
    };

}