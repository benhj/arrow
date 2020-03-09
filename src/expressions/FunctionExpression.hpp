/// (c) Ben Jones 2019 - present

#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>
#include <vector>

namespace arrow {
    
    class FunctionExpression : public Expression
    {
      public:
        FunctionExpression(long const lineNumber);
        FunctionExpression & withFunctionNameToken(Token token);
        FunctionExpression & 
        withExpressionCollection(std::shared_ptr<Expression> expression);
        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;
        std::string toString() const override;
        std::shared_ptr<Expression> const & getExpressionCollection() const;
        Token const & getName() const;
      private:
        Token m_nameToken;
        std::shared_ptr<Expression> m_expressionCollection;
    };

}