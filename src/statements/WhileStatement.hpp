/// (c) Ben Jones 2019 - present

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
        WhileStatement & withInnerStatement(std::shared_ptr<Statement> innerStatement);
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
        std::shared_ptr<Expression> const & getExpression() const;
        std::shared_ptr<Statement> const & getInnerStatement() const;
      private:
        Token m_keywordToken; // the while keyword
        std::shared_ptr<Expression> m_expression;
        std::shared_ptr<Statement> m_innerStatement;
    };

}