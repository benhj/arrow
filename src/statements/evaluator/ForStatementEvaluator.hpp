/// (c) Ben Jones 2019

#pragma once

#include "StatementEvaluator.hpp"
#include "statements/ForStatement.hpp"
#include "representation/Environment.hpp"
#include <memory>

namespace arrow {
    class ForStatementEvaluator final : public StatementEvaluator
    {
      public:
        explicit ForStatementEvaluator(ForStatement statement);
        StatementResult evaluate(Environment&) const override;
      private:
        ForStatement m_statement;
    };
}