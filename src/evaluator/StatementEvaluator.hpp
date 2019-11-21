#pragma once

#include "representation/Cache.hpp"
#include <memory>

namespace jasl {
    class StatementEvaluator
    {
      public:
        virtual void evaluate(Cache&) const = 0;
    };
}