#pragma once

#include "ArrowStatement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>

namespace arrow {
    class LengthStatement : public Statement
    {
      public:
        LengthStatement(ArrowStatement arrowStatement);
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
        LengthStatement const & withToken(Token token) const;
        Token getToken() const;
        std::shared_ptr<Expression> getExpression() const;
        Token getIdentifier() const;
      private:
        ArrowStatement mutable m_arrowStatement;
    };
}