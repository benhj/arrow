/// (c) Ben Jones 2019 - present

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
        ForStatement(long const lineNumber);
        ForStatement & withToken(Token token);
        ForStatement & withIndexer(Token token);
        ForStatement & withIdentifier(Token token);
        ForStatement & withInnerStatement(std::shared_ptr<Statement> innerStatement);
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
        Token getIndexer() const;
        Token getIdentifier() const;
        std::shared_ptr<Statement> getInnerStatement() const;
      private:
        Token m_keywordToken; // the while keyword
        Token m_indexer;
        Token m_identifier;
        std::shared_ptr<Statement> m_innerStatement;
    };

}