/// (c) Ben Jones 2025 - present

#pragma once

#include "Statement.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include <memory>

namespace arrow {
    
    class LoopContinueStatement final : public Statement
    {
      public:
        LoopContinueStatement(long const lineNumber);
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
    };

}