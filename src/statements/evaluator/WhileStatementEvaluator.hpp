/// (c) Ben Jones 2019

#pragma once

#include "StatementEvaluator.hpp"
#include "statements/WhileStatement.hpp"
#include "representation/Environment.hpp"
#include <memory>

namespace arrow {
    class WhileStatementEvaluator : public StatementEvaluator
    {
      public:
        explicit WhileStatementEvaluator(WhileStatement statement);
        StatementResult evaluate(Environment&) const override;
      private:
        WhileStatement m_statement;
    };
}