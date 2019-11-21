#include "Cache.hpp"

namespace jasl {

    Cache::Cache()
      : m_cacheStack{}
    {
        addCacheLayer();
    }

    Cache::CacheMap::iterator Cache::findAndRetrieveCached(std::string identifier) const
    {
        for (auto & layer : m_cacheStack) {
            auto found = layer.find(identifier);
            if(found != std::end(layer)) {
                return found;
            }
        }
        return CacheMap::iterator();
    }

    Type Cache::get(std::string identifier) const
    {
        auto found = findAndRetrieveCached(std::move(identifier));
        if(found == CacheMap::iterator()) { return {TypeDescriptor::None, false}; }
        return found->second;
    }

    void Cache::add(std::string identifier, Type const type)
    {
        auto found = findAndRetrieveCached(identifier);
        if(found != CacheMap::iterator()) {
            // Remove original instance of value
            if(found->second.m_descriptor == type.m_descriptor) {
                found->second.m_variantType.swap(type.m_variantType);
                return;
            } else {
                // error
                throw std::runtime_error("Can't update value with incompatible type.");
            }
        }
        // Add brand new instance
        m_cacheStack[0].emplace(identifier, type);
    }
    bool Cache::has(std::string identifier) const
    {
        auto found = findAndRetrieveCached(std::move(identifier));
        return found != CacheMap::iterator();
    }
    void Cache::remove(std::string identifier) const
    {
        for (auto & layer : m_cacheStack) {
            auto found = layer.find(identifier);
            if(found != std::end(layer)) {
                layer.erase(found);
                return;
            }
        }
    }
    void Cache::addCacheLayer()
    {
        m_cacheStack.emplace_front(CacheMap());
    }
}