#pragma once
#include "DecayType.hpp"
#include "parser/Node.hpp"
#include "types/Type.hpp"
#include <string>

namespace jasl {
    
    class Expression : public Node
    {
      public:
        Expression() : Node() {}

        /// Returns the evaluated type of the expression
        virtual Type evaluate() const = 0;

        /// During expression evaluation, we need
        /// to know the overaching type that it decays to
        virtual DecayType decayType() const = 0;

        /// For debugging purposes
        virtual std::string toString() const = 0;
    };


}