/// (c) Ben Jones 2020 - present

#include "PodType.hpp"
#include "Type.hpp"

namespace arrow {
    bool PodType::operator==(PodType const & other) const
    {
        return m_namedData == other.m_namedData;
    }
    bool PodType::operator!=(PodType const & other) const
    {
        return m_namedData != other.m_namedData;
    }

    void PodType::add(std::string identifier, Type t)
    {
        m_namedData.emplace(std::move(identifier), std::move(t));
    }

    bool PodType::has(std::string identifier)
    {
        auto found = m_namedData.find(std::move(identifier));
        if(found == std::end(m_namedData)) {
            return false;
        }
        return true;
    }

    Type PodType::get(std::string identifier)
    {
        auto found = m_namedData.find(std::move(identifier));
        return found->second;
    }
}