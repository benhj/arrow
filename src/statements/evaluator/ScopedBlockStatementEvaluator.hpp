/// (c) Ben Jones 2019 - present

#pragma once

#include "StatementEvaluator.hpp"
#include "statements/ScopedBlockStatement.hpp"
#include "representation/Environment.hpp"
#include <memory>

namespace arrow {
    class ScopedBlockStatementEvaluator : public StatementEvaluator
    {
      public:
        explicit ScopedBlockStatementEvaluator(ScopedBlockStatement statement);
        StatementResult evaluate(Environment&) const override;
      private:
        ScopedBlockStatement m_statement;
    };
}