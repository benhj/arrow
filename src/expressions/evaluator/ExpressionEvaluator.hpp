/// (c) Ben Jones 2019

#pragma once

#include "representation/Type.hpp"
#include "representation/Environment.hpp"
#include <memory>

namespace arrow {
    class ExpressionEvaluator
    {
      public:
        virtual Type evaluate(Environment&) const = 0;
    };
}