/// (c) Ben Jones 2019

#pragma once

#include "representation/Type.hpp"
#include "representation/Cache.hpp"
#include <memory>

namespace arrow {
    class ReceiverEvaluator
    {
      public:
        virtual void evaluate(Type, Cache&) const = 0;
    };
}