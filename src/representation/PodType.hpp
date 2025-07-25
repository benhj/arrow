/// (c) Ben Jones 2020 - present

#pragma once

#include <map>
#include <string>

namespace arrow {

    struct Type;

    /// To support rudimentary object orientation, include a
    /// pod (plain old data type). When fully integrated, this
    /// will allow the following syntax in arrow:
    /// pod p(one two three);
    /// p(1, [one two], 3) -> ident;
    /// ident:two -> ret;   ;;; ret is [one two]
    struct PodType {

        /// For storing the pod data. Every string key
        /// is the name of the bit of data.
        std::map<std::string, Type> m_namedData;
        bool operator==(PodType const & other) const;
        bool operator!=(PodType const & other) const;

        /// Add a data member
        void add(std::string identifier, Type t);

        /// Test membership
        bool has(std::string identifier);

        /// Retrieve a data member
        Type get(std::string identifier);
    };
}