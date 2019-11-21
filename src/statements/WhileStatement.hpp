#pragma once

#include "Statement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>
#include <vector>

namespace jasl {
    
    class WhileStatement : public Statement
    {
      public:
        WhileStatement();
        WhileStatement & withToken(Token token);
        WhileStatement & withExpression(std::shared_ptr<Expression> expression);
        void addBodyStatement(std::shared_ptr<Statement> bodyStatement);
        std::string toString() const override;
      private:
        Token m_keywordToken; // the while keyword
        std::shared_ptr<Expression> m_expression;
        std::vector<std::shared_ptr<Statement>> m_bodyStatements;
    };

}