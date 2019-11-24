#pragma once

#include "ArrowStatement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>

namespace arrow {
    /// represents a statement of the form
    /// args 1 -> a;
    /// to get a program argument.
    class ArgStatement : public Statement
    {
      public:
        ArgStatement(ArrowStatement arrowStatement);
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
        ArgStatement const & withToken(Token token) const;
        Token getToken() const;
        std::shared_ptr<Expression> getExpression() const;
        Token getIdentifier() const;
        ArrowStatement getArrowStatement() const;
      private:
        ArrowStatement mutable m_arrowStatement;
    };
}