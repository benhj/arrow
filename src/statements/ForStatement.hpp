#pragma once

#include "Statement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>
#include <vector>

namespace arrow {
    
    class ForStatement : public Statement
    {
      public:
        ForStatement();
        ForStatement & withToken(Token token);
        ForStatement & withIndexer(Token token);
        ForStatement & withIdentifier(Token token);
        void addBodyStatement(std::shared_ptr<Statement> bodyStatement);
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
        Token getIndexer() const;
        Token getIdentifier() const;
        std::vector<std::shared_ptr<Statement>> getBodyStatements() const;
      private:
        Token m_keywordToken; // the while keyword
        Token m_indexer;
        Token m_identifier;
        std::vector<std::shared_ptr<Statement>> m_bodyStatements;
    };

}