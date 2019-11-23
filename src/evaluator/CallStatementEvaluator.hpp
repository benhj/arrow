#pragma once

#include "StatementEvaluator.hpp"
#include "statements/CallStatement.hpp"
#include "representation/Cache.hpp"
#include <memory>

namespace arrow {
    class CallStatementEvaluator : public StatementEvaluator
    {
      public:
        explicit CallStatementEvaluator(CallStatement statement);
        void evaluate(Cache&) const override;
      private:
        CallStatement m_statement;
    };
}