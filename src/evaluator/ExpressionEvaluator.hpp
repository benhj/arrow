#pragma once

#include "representation/Type.hpp"
#include "representation/Cache.hpp"
#include <memory>

namespace jasl {
    class ExpressionEvaluator
    {
      public:
        virtual Type evaluate(Cache&) const = 0;
    };
}