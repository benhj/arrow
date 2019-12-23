/// (c) Ben Jones 2019 - present

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
        RepeatStatement & withInnerStatement(std::shared_ptr<Statement> innerStatement);
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
        std::shared_ptr<Expression> getExpression() const;
        std::shared_ptr<Statement> getInnerStatement() const;
      private:
        Token m_keywordToken; // the repeat keyword
        std::shared_ptr<Expression> m_expression;
        std::shared_ptr<Statement> m_innerStatement;
    };

}