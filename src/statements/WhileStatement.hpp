#pragma once

#include "Statement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>
#include <vector>

namespace arrow {
    
    class WhileStatement : public Statement
    {
      public:
        WhileStatement(long const lineNumber);
        WhileStatement & withToken(Token token);
        WhileStatement & withExpression(std::shared_ptr<Expression> expression);
        void addBodyStatement(std::shared_ptr<Statement> bodyStatement);
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
        std::shared_ptr<Expression> getExpression() const;
        std::vector<std::shared_ptr<Statement>> getBodyStatements() const;
      private:
        Token m_keywordToken; // the while keyword
        std::shared_ptr<Expression> m_expression;
        std::vector<std::shared_ptr<Statement>> m_bodyStatements;
    };

}