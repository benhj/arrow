/// (c) Ben Jones 2019 - present

#pragma once

#include "Statement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>
#include <vector>

namespace arrow {
    class FunctionStatement : public Statement
    {
      public:
        FunctionStatement(long const lineNumber);
        FunctionStatement & withToken(Token token);
        FunctionStatement & withNameToken(Token token);
        FunctionStatement & withReturnIdentifierToken(Token token);
        FunctionStatement & 
        withExpressionCollection(std::shared_ptr<Expression> expression);
        FunctionStatement & withInnerStatement(std::shared_ptr<Statement> innerStatement);
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
        Token getName() const;
        std::shared_ptr<Expression> getExpressionCollection() const;
        Token getReturnIdentifier() const;
        std::shared_ptr<Statement> getInnerStatement() const;
      private:
        Token m_keywordToken; // fn keyword
        Token m_typeToken; // return type
        Token m_nameToken;
        std::shared_ptr<Expression> m_expressionCollection;
        Token m_returnIdentifier;
        std::shared_ptr<Statement> m_innerStatement;
    };

}