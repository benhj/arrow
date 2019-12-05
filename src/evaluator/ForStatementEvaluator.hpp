/// (c) Ben Jones 2019

#pragma once

#include "StatementEvaluator.hpp"
#include "statements/ForStatement.hpp"
#include "representation/Cache.hpp"
#include <memory>

namespace arrow {
    class ForStatementEvaluator : public StatementEvaluator
    {
      public:
        explicit ForStatementEvaluator(ForStatement statement);
        void evaluate(Cache&) const override;
      private:
        ForStatement m_statement;
    };
}