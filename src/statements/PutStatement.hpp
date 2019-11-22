#pragma once

#include "ArrowStatement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>

namespace jasl {
    /// represents a statement of the form
    /// put 1 -> a;
    /// etc.
    class PutStatement : public Statement
    {
      public:
        PutStatement(ArrowStatement arrowStatement);
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
        PutStatement const & withToken(Token token) const;
        Token getToken() const;
        std::shared_ptr<Expression> getExpression() const;
        Token getIdentifier() const;
        ArrowStatement getArrowStatement() const;
      private:
        ArrowStatement mutable m_arrowStatement;
    };
}