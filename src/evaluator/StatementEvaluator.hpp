/// (c) Ben Jones 2019

#pragma once

#include "representation/Cache.hpp"
#include <memory>

namespace arrow {

    enum class StatementResult {
        Break,
        Return,
        Continue
    };

    class StatementEvaluator
    {
      public:
        virtual StatementResult evaluate(Cache&) const = 0;
    };
}