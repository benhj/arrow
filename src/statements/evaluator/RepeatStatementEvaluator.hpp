/// (c) Ben Jones 2019

#pragma once

#include "StatementEvaluator.hpp"
#include "statements/RepeatStatement.hpp"
#include "representation/Environment.hpp"
#include <memory>

namespace arrow {
    class RepeatStatementEvaluator : public StatementEvaluator
    {
      public:
        explicit RepeatStatementEvaluator(RepeatStatement statement);
        StatementResult evaluate(Environment&) const override;
      private:
        RepeatStatement m_statement;
    };
}