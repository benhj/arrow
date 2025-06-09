/// (c) Ben Jones 2019 - present

#pragma once

#include "real.hpp"
#include <cstdint>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace arrow {

    /// Represents the set of built-in types
    struct Type;
    struct PodType;
    using VariantType = std::variant<int64_t,
                                     real, std::string,
                                     uint8_t, std::vector<Type>,
                                     std::vector<bool>,
                                     std::vector<int64_t>,
                                     std::vector<real>,
                                     std::vector<std::string>,
                                     char, bool,
                                     std::vector<char>,
                                     std::map<std::string, Type>,
                                     PodType,
                                     std::vector<PodType>>;
}