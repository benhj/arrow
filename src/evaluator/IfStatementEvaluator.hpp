/// (c) Ben Jones 2019

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
        bool evaluate(Cache&) const override;
      private:
        IfStatement m_statement;
    };
}