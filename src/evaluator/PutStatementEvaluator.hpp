#pragma once

#include "StatementEvaluator.hpp"
#include "statements/PutStatement.hpp"
#include "representation/Cache.hpp"
#include <memory>

namespace arrow {
    class PutStatementEvaluator : public StatementEvaluator
    {
      public:
        PutStatementEvaluator(PutStatement statement);
        void evaluate(Cache&) const override;
      private:
        PutStatement m_statement;
    };
}