/// (c) Ben Jones 2019

#pragma once

#include "representation/Type.hpp"
#include "representation/Environment.hpp"
#include <memory>

namespace arrow {
    class ReceiverEvaluator
    {
      public:
        virtual void evaluate(Type, Environment&) const = 0;
    };
}