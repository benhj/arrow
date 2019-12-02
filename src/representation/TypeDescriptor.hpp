#pragma once

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

namespace arrow {

    /// Describes a given type
    enum class TypeDescriptor {
        Int, Bool, Real, Byte, String, List, BracketlessList,
        Ints, Bools, Reals, Bytes, Strings, None, SingleEqual,
        DoubleEqual, ExpressionCollection, BracedExpressionCollection,
        Nil
    };

}