#pragma once
#include "DecayType.hpp"
#include "parser/Node.hpp"
#include <string>

namespace jasl {
    
    class Expression : public Node
    {
      public:
        Expression() : Node() {}

        /// During expression evaluation, we need
        /// to know the overaching type that it decays to
        virtual DecayType decayType() const = 0;

        /// For debugging purposes
        virtual std::string toString() const = 0;
    };


}