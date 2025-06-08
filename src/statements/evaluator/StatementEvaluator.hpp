/// (c) Ben Jones 2019 - present

#pragma once

#include "representation/Environment.hpp"
#include <memory>

namespace arrow {

    enum class StatementResult {
        Break,
        Return,
        Continue,
        LoopContinue, // Since Continue is used to mark end of successful eval
        Exit
    };

    class StatementEvaluator
    {
      public:
        virtual StatementResult evaluate(Environment&) const = 0;
    };
}