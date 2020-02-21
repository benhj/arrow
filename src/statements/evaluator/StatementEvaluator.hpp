/// (c) Ben Jones 2019

#pragma once

#include "representation/Environment.hpp"
#include <memory>

namespace arrow {

    enum class StatementResult {
        Break,
        Return,
        Continue,
        Exit
    };

    class StatementEvaluator
    {
      public:
        virtual StatementResult evaluate(Environment&) const = 0;
    };
}