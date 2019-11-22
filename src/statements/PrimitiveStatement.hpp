#pragma once

#include "ArrowStatement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>

namespace arrow {
    /// represents a statement of the form
    /// int 1 -> a;
    /// real 2.0 -> a;
    /// etc.
    /// Basically an ArrowStatement but for
    /// setting types.
    class PrimitiveStatement : public Statement
    {
      public:
        PrimitiveStatement(ArrowStatement arrowStatement);
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
        PrimitiveStatement const & withToken(Token token) const;
        Token getToken() const;
        std::shared_ptr<Expression> getExpression() const;
        Token getIdentifier() const;
      private:
        ArrowStatement mutable m_arrowStatement;
    };
}