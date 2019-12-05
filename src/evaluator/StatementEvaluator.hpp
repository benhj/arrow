/// (c) Ben Jones 2019

#pragma once

#include "representation/Cache.hpp"
#include <memory>

namespace arrow {
    class StatementEvaluator
    {
      public:
        virtual void evaluate(Cache&) const = 0;
    };
}