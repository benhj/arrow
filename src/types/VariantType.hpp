#pragma once

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

namespace jasl {

    /// Represents the set of built-in types
    using VariantType = std::variant<bool, int64_t,
                                     double, std::string,
                                     uint8_t, std::vector<std::string>>;

}