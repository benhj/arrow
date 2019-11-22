#pragma once

#include "ElseIfStatement.hpp"
#include "ElseStatement.hpp"
#include "Statement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>
#include <vector>

namespace jasl {
    
    /// Note, an IfStatement can contain optional
    /// ElseIfStatement parts an an optional
    /// ElseStatement part
    class IfStatement : public Statement
    {
      public:
        IfStatement();
        IfStatement & withToken(Token token);
        IfStatement & withExpression(std::shared_ptr<Expression> expression);
        std::shared_ptr<Expression> getExpression() const;
        void addBodyStatement(std::shared_ptr<Statement> bodyStatement);
        void addElseIfPart(std::shared_ptr<ElseIfStatement> elseIfPart);
        IfStatement & withElsePart(std::shared_ptr<ElseStatement> elsePart);
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
        std::vector<std::shared_ptr<Statement>> getBodyStatements() const;
        std::vector<std::shared_ptr<ElseIfStatement>> getElseIfParts() const;
        std::shared_ptr<ElseStatement> getElsePart() const;
      private:
        Token m_keywordToken; // the if keyword
        std::shared_ptr<Expression> m_expression;
        std::vector<std::shared_ptr<Statement>> m_bodyStatements;
        std::vector<std::shared_ptr<ElseIfStatement>> m_elseIfParts;
        std::shared_ptr<ElseStatement> m_elsePart;
    };

}