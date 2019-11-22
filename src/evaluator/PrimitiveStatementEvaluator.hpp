#pragma once

#include "StatementEvaluator.hpp"
#include "statements/PrimitiveStatement.hpp"
#include "representation/Cache.hpp"
#include <memory>

namespace arrow {
    class PrimitiveStatementEvaluator : public StatementEvaluator
    {
      public:
        PrimitiveStatementEvaluator(PrimitiveStatement statement);
        void evaluate(Cache&) const override;
      private:
        PrimitiveStatement m_statement;
    };
}