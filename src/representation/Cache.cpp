#include "Cache.hpp"
#include "parser/LanguageException.hpp"

namespace arrow {

    std::deque<Type> Cache::m_returnStack;

    Cache::Cache()
      : m_cacheStack{}
    {
        pushCacheLayer();
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

    Type Cache::get(Token identifier) const
    {
        auto found = findAndRetrieveCached(identifier.raw);
        if(found == CacheMap::iterator()) { return {TypeDescriptor::None, false}; }
        return found->second;
    }

    void Cache::add(Token identifier, Type const type)
    {
        auto found = findAndRetrieveCached(identifier.raw);
        auto error{false};
        if(found != CacheMap::iterator()) {
            // Remove original instance of value
            if(found->second.m_descriptor == type.m_descriptor) {
                found->second.m_variantType.swap(type.m_variantType);
                return;
            } else {
                error = true;
            }
        }
        if(error) {
            throw LanguageException("Incompatible type", identifier.lineNumber);
        }
        // Add brand new instance
        m_cacheStack[0].emplace(identifier.raw, type);
    }
    bool Cache::has(Token identifier) const
    {
        auto found = findAndRetrieveCached(identifier.raw);
        return found != CacheMap::iterator();
    }
    void Cache::remove(Token identifier) const
    {
        for (auto & layer : m_cacheStack) {
            auto found = layer.find(identifier.raw);
            if(found != std::end(layer)) {
                layer.erase(found);
                return;
            }
        }
    }
    void Cache::setElementInContainer(Token identifier,
                                      int const index,
                                      Type const type)
    {
        auto found = findAndRetrieveCached(identifier.raw);
        auto casted = std::get<std::vector<Type>>(found->second.m_variantType);
        if(index >= casted.size()) {
            throw LanguageException("Index out of range", identifier.lineNumber);
        }
        casted[index].m_variantType.swap(type.m_variantType);
        found->second.m_variantType = casted;
    }
    void Cache::pushCacheLayer()
    {
        m_cacheStack.emplace_front(CacheMap());
    }
    void Cache::popCacheLayer()
    {
        m_cacheStack.pop_front();
    }

    void Cache::pushReturnValue(Type t)
    {
        m_returnStack.push_front(std::move(t));
    }
    Type Cache::getAndPopReturnValue()
    {
        auto t = m_returnStack[0];
        m_returnStack.pop_front();
        return t;
    }
    void Cache::pushProgramArgument(Type arg)
    {
        m_programArguments.push_back(std::move(arg));
    }

    Type Cache::getProgramArgument(int64_t const index) const
    {
        return m_programArguments[index];
    }
}