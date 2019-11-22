#pragma once

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

namespace jasl {

    /// Describes a given type
    enum class TypeDescriptor {
        Int, Bool, Real, Byte, String, List, BracketlessList, None
    };

}