/// (c) Ben Jones 2019 - present

#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace arrow {
    
    /// An expression of the form
    /// identifier[expression], e.g. a[1]
    class IndexExpression : public Expression
    {
      public:
        IndexExpression(long const lineNumber);

        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;

        IndexExpression & withIdentifierToken(Token identifier);
        IndexExpression & withIndexExpression(std::shared_ptr<Expression> identifier);

        Token const & getIdentifierToken() const;
        std::shared_ptr<Expression> getIndexExpression() const;

        std::string toString() const override;

      private:
        Token m_identifier;
        std::shared_ptr<Expression> m_indexExpression;
    };

}