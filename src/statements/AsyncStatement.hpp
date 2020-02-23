/// (c) Ben Jones 2019 - present

#pragma once

#include "Statement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>
#include <utility>

namespace arrow {
    class AsyncStatement : public Statement
    {
      public:
        AsyncStatement(long const lineNumber);
        AsyncStatement & withToken(Token token);
        void addBodyStatement(std::shared_ptr<Statement> bodyStatement);
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
        std::vector<std::shared_ptr<Statement>> getBodyStatements() const;
      private:
        Token m_keywordToken; // start keyword
        std::vector<std::shared_ptr<Statement>> m_bodyStatements;
    };

}