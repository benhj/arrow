/// (c) Ben Jones 2019 - present

#include "Cache.hpp"
#include "parser/LanguageException.hpp"

namespace arrow {

    namespace {
        template <typename T>
        std::vector<T> getUpdatedArray(VariantType vt,
                                       Type element,
                                       int const index,
                                       long const lineNumber)
        {
            auto casted = std::get<std::vector<T>>(vt);
            if(index >= casted.size()) {
                throw LanguageException("Index out of range", lineNumber);
            }
            if constexpr(std::is_same_v<T, Type>) {
                casted[index].m_variantType.swap(element.m_variantType);
            } else {
                auto const val = std::get<T>(element.m_variantType);
                casted[index] = val;
            }
            return casted;
        }

        template <typename T>
        std::pair<bool, std::vector<T>> tryErase(VariantType vt,
                                                 int const index,
                                                 long const lineNumber)
        {
            try {
                auto casted = std::get<std::vector<T>>(vt);
                if(index >= casted.size()) {
                    throw LanguageException("Index out of range", lineNumber);
                }
                casted.erase(std::begin(casted) + index);
                return {true, casted};
            } catch (...) {
                return {false, std::vector<T>()};
            }
        }
    }

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
        if(found != CacheMap::iterator()) {
            // Remove original instance of value
            found->second.m_variantType.swap(type.m_variantType);
            return;
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

    void Cache::pushBackContainerElement(Token identifier, Type const type)
    {
        auto found = findAndRetrieveCached(identifier.raw);
        
        if(found->second.m_descriptor == TypeDescriptor::Ints) {
            auto & casted = std::get<std::vector<int64_t>>(found->second.m_variantType);
            auto tval = std::get<int64_t>(type.m_variantType);
            casted.push_back(tval);
        } else if(found->second.m_descriptor == TypeDescriptor::Reals) {
            auto & casted = std::get<std::vector<long double>>(found->second.m_variantType);
            auto tval = std::get<long double>(type.m_variantType);
            casted.push_back(tval);
        } else if(found->second.m_descriptor == TypeDescriptor::Strings) {
            auto & casted = std::get<std::vector<std::string>>(found->second.m_variantType);
            auto tval = std::get<std::string>(type.m_variantType);
            casted.push_back(tval);
        } else if(found->second.m_descriptor == TypeDescriptor::Bytes) {
            auto & casted = std::get<std::vector<char>>(found->second.m_variantType);
            auto tval = std::get<char>(type.m_variantType);
            casted.push_back(tval);
        } else if(found->second.m_descriptor == TypeDescriptor::Bools) {
            auto & casted = std::get<std::vector<bool>>(found->second.m_variantType);
            auto tval = std::get<bool>(type.m_variantType);
            casted.push_back(tval);
        } else if(found->second.m_descriptor == TypeDescriptor::List) {
            auto & casted = std::get<std::vector<Type>>(found->second.m_variantType);
            casted.push_back(type);
        } 
    }

    void Cache::setElementInContainer(Token identifier,
                                      int const index,
                                      Type const type)
    {
        auto found = findAndRetrieveCached(identifier.raw);
        try {
            if(type.m_descriptor == TypeDescriptor::Int) {
                auto casted = getUpdatedArray<int64_t>(found->second.m_variantType, type,
                                                       index, identifier.lineNumber);
                found->second.m_variantType = casted;
                return;
            } else if(type.m_descriptor == TypeDescriptor::Real) {
                auto casted = getUpdatedArray<long double>(found->second.m_variantType, type,
                                                           index, identifier.lineNumber);
                found->second.m_variantType = casted;
                return;
            } else if(type.m_descriptor == TypeDescriptor::String) {
                auto casted = getUpdatedArray<std::string>(found->second.m_variantType, type,
                                                           index, identifier.lineNumber);
                found->second.m_variantType = casted;
                return;
            } else if(type.m_descriptor == TypeDescriptor::Byte) {
                auto casted = getUpdatedArray<char>(found->second.m_variantType, type,
                                                    index, identifier.lineNumber);
                found->second.m_variantType = casted;
                return;
            } else if(type.m_descriptor == TypeDescriptor::Bool) {
                auto casted = getUpdatedArray<bool>(found->second.m_variantType, type,
                                                    index, identifier.lineNumber);
                found->second.m_variantType = casted;
                return;
            } 
        } catch (...) {
            // if casting threw, it must be because of bad type conversion
            // meaning we can instead try Type as the list element type.
        }
        auto casted = getUpdatedArray<Type>(found->second.m_variantType, type,
                                            index, identifier.lineNumber);
        found->second.m_variantType = casted;
    }

    void Cache::eraseElementInContainer(Token identifier,
                                        int const index)
    {
        auto found = findAndRetrieveCached(identifier.raw);
        {
            auto result = tryErase<int64_t>(found->second.m_variantType, index, identifier.lineNumber);
            if(result.first) {
                found->second.m_variantType = result.second;
                return;
            }
        }
        {
            auto result = tryErase<long double>(found->second.m_variantType, index, identifier.lineNumber);
            if(result.first) {
                found->second.m_variantType = result.second;
                return;
            }
        }
        {
            auto result = tryErase<bool>(found->second.m_variantType, index, identifier.lineNumber);
            if(result.first) {
                found->second.m_variantType = result.second;
                return;
            }
        }
        {
            auto result = tryErase<char>(found->second.m_variantType, index, identifier.lineNumber);
            if(result.first) {
                found->second.m_variantType = result.second;
                return;
            }
        }
        {
            auto result = tryErase<std::string>(found->second.m_variantType, index, identifier.lineNumber);
            if(result.first) {
                found->second.m_variantType = result.second;
                return;
            }
        }
        {
            auto result = tryErase<Type>(found->second.m_variantType, index, identifier.lineNumber);
            if(result.first) {
                found->second.m_variantType = result.second;
                return;
            }
        }
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