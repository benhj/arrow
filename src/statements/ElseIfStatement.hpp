/// (c) Ben Jones 2019 - present

#pragma once

#include "Statement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>
#include <vector>

namespace arrow {
    
    /// Note, an ElseIfStatement can only be part
    /// of an IfStatement
    class ElseIfStatement final : public Statement
    {
      public:
        ElseIfStatement(long const lineNumber);
        ElseIfStatement & withToken(Token token);
        ElseIfStatement & withExpression(std::shared_ptr<Expression> expression);
        std::shared_ptr<Expression> const & getExpression() const;
        ElseIfStatement & withInnerStatement(std::shared_ptr<Statement> innerStatement);
        std::shared_ptr<Statement> const & getInnerStatement() const;

        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;

      private:
        Token m_keywordToken; // the elseif keyword
        std::shared_ptr<Expression> m_expression;
        std::shared_ptr<Statement> m_innerStatement;
    };

}