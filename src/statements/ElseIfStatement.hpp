#pragma once

#include "Statement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>
#include <vector>

namespace arrow {
    
    /// Note, an ElseIfStatement can only be part
    /// of an IfStatement
    class ElseIfStatement : public Statement
    {
      public:
        ElseIfStatement();
        ElseIfStatement & withToken(Token token);
        ElseIfStatement & withExpression(std::shared_ptr<Expression> expression);
        std::shared_ptr<Expression> getExpression() const;
        void addBodyStatement(std::shared_ptr<Statement> bodyStatement);
        std::vector<std::shared_ptr<Statement>> getBodyStatements() const;

        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;

      private:
        Token m_keywordToken; // the elseif keyword
        std::shared_ptr<Expression> m_expression;
        std::vector<std::shared_ptr<Statement>> m_bodyStatements;
    };

}