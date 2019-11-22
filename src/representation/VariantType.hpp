#pragma once

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

namespace arrow {

    /// Represents the set of built-in types
    class Type;
    using VariantType = std::variant<bool, int64_t,
                                     long double, std::string,
                                     uint8_t, std::vector<Type>>;
}