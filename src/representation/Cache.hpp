#pragma once

#include "Type.hpp"
#include "TypeDescriptor.hpp"
#include <deque>
#include <map>
#include <string>

namespace jasl {

    class Cache
    {
      public:
        using CacheMap = std::map<std::string, Type>;
        Cache();
        Type get(std::string identifier) const;
        void add(std::string identifier, Type const type);
        bool has(std::string identifier, TypeDescriptor const & descriptor) const;
        void remove(std::string identifier) const;
        void addCacheLayer();
      private:

        CacheMap::iterator findAndRetrieveCached(std::string identifier) const;

        std::deque<CacheMap> mutable m_cacheStack;
    };
}