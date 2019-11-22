#pragma once

#include "StatementEvaluator.hpp"
#include "statements/WhileStatement.hpp"
#include "representation/Cache.hpp"
#include <memory>

namespace jasl {
    class WhileStatementEvaluator : public StatementEvaluator
    {
      public:
        explicit WhileStatementEvaluator(WhileStatement statement);
        void evaluate(Cache&) const override;
      private:
        WhileStatement m_statement;
    };
}