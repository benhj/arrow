/// (c) Ben Jones 2020

#pragma once

#include "StatementEvaluator.hpp"
#include "statements/PodStatement.hpp"
#include "representation/Environment.hpp"
#include <memory>

namespace arrow {
    class PodStatementEvaluator : public StatementEvaluator
    {
      public:
        explicit PodStatementEvaluator(PodStatement statement);
        StatementResult evaluate(Environment&) const override;
      private:
        PodStatement m_statement;
    };
}