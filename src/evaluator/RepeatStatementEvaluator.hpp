#pragma once

#include "StatementEvaluator.hpp"
#include "statements/RepeatStatement.hpp"
#include "representation/Cache.hpp"
#include <memory>

namespace jasl {
    class RepeatStatementEvaluator : public StatementEvaluator
    {
      public:
        explicit RepeatStatementEvaluator(RepeatStatement statement);
        void evaluate(Cache&) const override;
      private:
        RepeatStatement m_statement;
    };
}