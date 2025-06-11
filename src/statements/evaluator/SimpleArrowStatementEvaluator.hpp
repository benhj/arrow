/// (c) Ben Jones 2019

#pragma once

#include "StatementEvaluator.hpp"
#include "statements/SimpleArrowStatement.hpp"
#include "representation/Environment.hpp"
#include <memory>

namespace arrow {
    class SimpleArrowStatementEvaluator final : public StatementEvaluator
    {
      public:
        SimpleArrowStatementEvaluator(SimpleArrowStatement statement);
        StatementResult evaluate(Environment&) const override;
      private:
        SimpleArrowStatement m_statement;
    };
}