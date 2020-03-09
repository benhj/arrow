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
        ForStatement & addIndexer(Token token);
        ForStatement & withIdentifier(Token token);
        ForStatement & withInnerStatement(std::shared_ptr<Statement> innerStatement);
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
        std::vector<Token> const & getIndices() const;
        Token const & getIdentifier() const;
        std::shared_ptr<Statement> const & getInnerStatement() const;
      private:
        Token m_keywordToken; // the while keyword
        std::vector<Token> m_indices;
        Token m_identifier;
        std::shared_ptr<Statement> m_innerStatement;
    };

}