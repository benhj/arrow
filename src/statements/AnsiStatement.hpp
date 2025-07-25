/// (c) Ben Jones 2019 - present

#pragma once

#include "ArrowlessStatement.hpp"
#include <memory>

namespace arrow {
    
    class AnsiStatement final : public Statement
    {
      public:
        AnsiStatement(long const lineNumber,
                      std::shared_ptr<ArrowlessStatement> statement);
        Token getToken() const;
        std::shared_ptr<Expression> const & getExpression() const;
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
      private:
        std::shared_ptr<ArrowlessStatement> m_statement;
    };
}