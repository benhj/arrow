#pragma once

#include "StatementEvaluator.hpp"
#include "statements/IfStatement.hpp"
#include "representation/Cache.hpp"
#include <memory>

namespace arrow {
    class IfStatementEvaluator : public StatementEvaluator
    {
      public:
        explicit IfStatementEvaluator(IfStatement statement);
        void evaluate(Cache&) const override;
      private:
        IfStatement m_statement;
    };
}