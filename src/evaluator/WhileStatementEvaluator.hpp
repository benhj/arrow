/// (c) Ben Jones 2019

#pragma once

#include "StatementEvaluator.hpp"
#include "statements/WhileStatement.hpp"
#include "representation/Cache.hpp"
#include <memory>

namespace arrow {
    class WhileStatementEvaluator : public StatementEvaluator
    {
      public:
        explicit WhileStatementEvaluator(WhileStatement statement);
        bool evaluate(Cache&) const override;
      private:
        WhileStatement m_statement;
    };
}