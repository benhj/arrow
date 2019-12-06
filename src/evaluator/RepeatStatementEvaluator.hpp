/// (c) Ben Jones 2019

#pragma once

#include "StatementEvaluator.hpp"
#include "statements/RepeatStatement.hpp"
#include "representation/Cache.hpp"
#include <memory>

namespace arrow {
    class RepeatStatementEvaluator : public StatementEvaluator
    {
      public:
        explicit RepeatStatementEvaluator(RepeatStatement statement);
        bool evaluate(Cache&) const override;
      private:
        RepeatStatement m_statement;
    };
}