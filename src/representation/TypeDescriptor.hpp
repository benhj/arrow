/// (c) Ben Jones 2019 - present

#pragma once

namespace arrow {

    /// Describes a given type
    enum class TypeDescriptor {
        Int, Bool, Real, Byte, String, List, BracketlessList,
        Ints, Bools, Reals, Bytes, Strings, None, SingleEqual,
        DoubleEqual, ExpressionCollection, BracedExpressionCollection,
        Nil, QString, QQString, ListWord, Map
    };

}