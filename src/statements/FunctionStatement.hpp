#pragma once

#include "Statement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>
#include <vector>

namespace jasl {
    class FunctionStatement : public Statement
    {
      public:
        FunctionStatement();
        FunctionStatement & withToken(Token token);
        FunctionStatement & withTypeToken(Token token);
        FunctionStatement & withNameToken(Token token);
        FunctionStatement & withReturnIdentifierToken(Token token);
        FunctionStatement & 
        withExpressionCollection(std::shared_ptr<Expression> expression);
        void addBodyStatement(std::shared_ptr<Statement> bodyStatement);
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
      private:
        Token m_keywordToken; // fn keyword
        Token m_typeToken; // return type
        Token m_nameToken;
        std::shared_ptr<Expression> m_expressionCollection;
        Token m_returnIdentifier;
        std::vector<std::shared_ptr<Statement>> m_bodyStatements;
    };

}