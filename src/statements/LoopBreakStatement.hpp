/// (c) Ben Jones 2019 - present

#pragma once

#include "Statement.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include <memory>

namespace arrow {
    
    class LoopBreakStatement final : public Statement
    {
      public:
        LoopBreakStatement(long const lineNumber);
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
    };

}