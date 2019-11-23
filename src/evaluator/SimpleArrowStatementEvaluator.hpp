#pragma once

#include "StatementEvaluator.hpp"
#include "statements/SimpleArrowStatement.hpp"
#include "representation/Cache.hpp"
#include <memory>

namespace arrow {
    class SimpleArrowStatementEvaluator : public StatementEvaluator
    {
      public:
        SimpleArrowStatementEvaluator(SimpleArrowStatement statement);
        void evaluate(Cache&) const override;
      private:
        SimpleArrowStatement m_statement;
    };
}