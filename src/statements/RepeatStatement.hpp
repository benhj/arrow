#pragma once

#include "Statement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>
#include <vector>

namespace arrow {
    
    class RepeatStatement : public Statement
    {
      public:
        RepeatStatement(long const lineNumber);
        RepeatStatement & withToken(Token token);
        RepeatStatement & withExpression(std::shared_ptr<Expression> expression);
        void addBodyStatement(std::shared_ptr<Statement> bodyStatement);
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
        std::shared_ptr<Expression> getExpression() const;
        std::vector<std::shared_ptr<Statement>> getBodyStatements() const;
      private:
        Token m_keywordToken; // the repeat keyword
        std::shared_ptr<Expression> m_expression;
        std::vector<std::shared_ptr<Statement>> m_bodyStatements;
    };

}