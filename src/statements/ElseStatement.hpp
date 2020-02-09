/// (c) Ben Jones 2019 - present

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
        ElseStatement(long const lineNumber, std::ostream&);
        ElseStatement & withToken(Token token);
        ElseStatement & withInnerStatement(std::shared_ptr<Statement> innerStatement);
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
        std::shared_ptr<Statement> getInnerStatement() const;
      private:
        Token m_keywordToken; // the else keyword
        std::shared_ptr<Statement> m_innerStatement;
    };

}