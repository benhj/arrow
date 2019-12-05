/// (c) Ben Jones 2019 - present

#pragma once

#include "Statement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>

namespace arrow {
    
    class ArrowlessStatement : public Statement
    {
      public:
        ArrowlessStatement(long const lineNumber);
        ArrowlessStatement & withToken(Token token);
        ArrowlessStatement & withExpression(std::shared_ptr<Expression> expression);
        Token getToken() const;
        std::shared_ptr<Expression> getExpression() const;
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
      private:
        Token m_keywordToken; // the keyword (e.g. prn)
        std::shared_ptr<Expression> m_expression;
    };

}