/// (c) Ben Jones 2019

#pragma once

#include "representation/Type.hpp"
#include "representation/Cache.hpp"
#include <memory>

namespace arrow {
    class ExpressionEvaluator
    {
      public:
        virtual Type evaluate(Cache&) const = 0;
    };
}