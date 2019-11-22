#pragma once

#include "Statement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>
#include <utility>

namespace jasl {
    class StartStatement : public Statement
    {
      public:
        StartStatement();
        StartStatement & withToken(Token token);
        void addBodyStatement(std::shared_ptr<Statement> bodyStatement);
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
      private:
        Token m_keywordToken; // start keyword
        std::vector<std::shared_ptr<Statement>> m_bodyStatements;
    };

}