#pragma once

#include "Statement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>
#include <vector>

namespace jasl {
    
    class ForStatement : public Statement
    {
      public:
        ForStatement();
        ForStatement & withToken(Token token);
        ForStatement & withIdentifierA(Token identifier);
        ForStatement & withIdentifierB(Token identifier);
        void addBodyStatement(std::shared_ptr<Statement> bodyStatement);
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
      private:
        Token m_keywordToken; // the for keyword
        Token m_identifierA;
        Token m_identifierB;
        std::vector<std::shared_ptr<Statement>> m_bodyStatements;
    };

}