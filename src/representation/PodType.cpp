/// (c) Ben Jones 2020 - present

#include "PodType.hpp"
#include "Type.hpp"

namespace arrow {
    bool PodType::operator==(PodType const & other) const
    {
        return m_namedData == other.m_namedData;
    }
}