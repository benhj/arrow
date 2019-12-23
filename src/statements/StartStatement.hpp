/// (c) Ben Jones 2019 - present

#pragma once

#include "Statement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>
#include <utility>

namespace arrow {
    class StartStatement : public Statement
    {
      public:
        StartStatement(long const lineNumber);
        StartStatement & withToken(Token token);
        StartStatement & withInnerStatement(std::shared_ptr<Statement> innerStatement);
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
        std::shared_ptr<Statement> getInnerStatement() const;
      private:
        Token m_keywordToken; // start keyword
        std::shared_ptr<Statement> m_innerStatement;
    };

}