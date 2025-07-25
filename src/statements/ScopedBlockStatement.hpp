/// (c) Ben Jones 2019 - present

#pragma once

#include "Statement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>
#include <vector>

namespace arrow {
    
    class ScopedBlockStatement final : public Statement
    {
      public:
        ScopedBlockStatement(long const lineNumber);
        void addBodyStatement(std::shared_ptr<Statement> bodyStatement);
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
        std::vector<std::shared_ptr<Statement>> const & getBodyStatements() const;
      private:
        std::vector<std::shared_ptr<Statement>> m_bodyStatements;
    };

}