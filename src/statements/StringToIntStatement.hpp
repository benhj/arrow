/// (c) Ben Jones 2019 - present

// TODO: obsolete code (convert to a built-in expression type)

/*
#pragma once

#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>

namespace arrow {
    class StringToIntStatement : public Statement
    {
      public:
        StringToIntStatement(long const lineNumber, ArrowStatement arrowStatement);
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
        StringToIntStatement const & withToken(Token token) const;
        Token getToken() const;
        std::shared_ptr<Expression> getExpression() const;
        Token getIdentifier() const;
      private:
        ArrowStatement mutable m_arrowStatement;
        
    };
}
*/