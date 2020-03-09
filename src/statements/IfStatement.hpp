/// (c) Ben Jones 2019 - present

#pragma once

#include "ElseIfStatement.hpp"
#include "ElseStatement.hpp"
#include "Statement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>
#include <vector>

namespace arrow {
    
    /// Note, an IfStatement can contain optional
    /// ElseIfStatement parts an an optional
    /// ElseStatement part
    class IfStatement : public Statement
    {
      public:
        IfStatement(long const lineNumber);
        IfStatement & withToken(Token token);
        IfStatement & withExpression(std::shared_ptr<Expression> expression);
        std::shared_ptr<Expression> const & getExpression() const;
        IfStatement & withInnerStatement(std::shared_ptr<Statement> innerStatement);
        void addElseIfPart(std::shared_ptr<ElseIfStatement> elseIfPart);
        IfStatement & withElsePart(std::shared_ptr<ElseStatement> elsePart);
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
        std::shared_ptr<Statement> const & getInnerStatement() const;
        std::vector<std::shared_ptr<ElseIfStatement>> const &  getElseIfParts() const;
        std::shared_ptr<ElseStatement> const & getElsePart() const;
      private:
        Token m_keywordToken; // the if keyword
        std::shared_ptr<Expression> m_expression;
        std::shared_ptr<Statement> m_innerStatement;
        std::vector<std::shared_ptr<ElseIfStatement>> m_elseIfParts;
        std::shared_ptr<ElseStatement> m_elsePart;
    };

}