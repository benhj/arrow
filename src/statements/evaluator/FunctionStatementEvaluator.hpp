/// (c) Ben Jones 2019

#pragma once

#include "StatementEvaluator.hpp"
#include "statements/FunctionStatement.hpp"
#include "representation/Environment.hpp"
#include <memory>

namespace arrow {
    class FunctionStatementEvaluator : public StatementEvaluator
    {
      public:
        explicit FunctionStatementEvaluator(FunctionStatement statement);
        StatementResult evaluate(Environment&) const override;
      private:
        FunctionStatement m_statement;
    };
}