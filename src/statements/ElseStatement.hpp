#pragma once

#include "Statement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>
#include <vector>

namespace arrow {
    
    /// Note, an ElseStatement can only be part
    /// of an IfStatement or ElseIfStatement
    class ElseStatement : public Statement
    {
      public:
        ElseStatement(long const lineNumber);
        ElseStatement & withToken(Token token);
        void addBodyStatement(std::shared_ptr<Statement> bodyStatement);
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
        std::vector<std::shared_ptr<Statement>> getBodyStatements() const;
      private:
        Token m_keywordToken; // the else keyword
        std::vector<std::shared_ptr<Statement>> m_bodyStatements;
    };

}