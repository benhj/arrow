#include "Cache.hpp"

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

            // Push back if array
            if(found->second.m_descriptor == TypeDescriptor::Ints) {
                if(type.m_descriptor != TypeDescriptor::Int) {
                    error = true;
                }
                auto vec = std::get<std::vector<int64_t>>(found->second.m_variantType);
                auto deduced = std::get<int64_t>(type.m_variantType);
                vec.push_back(deduced);
                VariantType newv{vec};
                found->second.m_variantType.swap(newv);
                return;
            } else if (found->second.m_descriptor == TypeDescriptor::Reals) {
                if(type.m_descriptor != TypeDescriptor::Real) {
                    error = true;
                }
                auto vec = std::get<std::vector<long double>>(found->second.m_variantType);
                auto deduced = std::get<long double>(type.m_variantType);
                vec.push_back(deduced);
                VariantType newv{vec};
                found->second.m_variantType.swap(newv);
                return;
            } else if (found->second.m_descriptor == TypeDescriptor::Bools) {
                if(type.m_descriptor != TypeDescriptor::Bool) {
                    error = true;
                }
                auto vec = std::get<std::vector<bool>>(found->second.m_variantType);
                auto deduced = std::get<bool>(type.m_variantType);
                vec.push_back(deduced);
                VariantType newv{vec};
                found->second.m_variantType.swap(newv);
                return;
            } else if (found->second.m_descriptor == TypeDescriptor::Strings) {
                if(type.m_descriptor != TypeDescriptor::String) {
                    error = true;
                }
                auto vec = std::get<std::vector<std::string>>(found->second.m_variantType);
                auto deduced = std::get<std::string>(type.m_variantType);
                vec.push_back(deduced);
                VariantType newv{vec};
                found->second.m_variantType.swap(newv);
                return;
            }

            // Remove original instance of value
            if(found->second.m_descriptor == type.m_descriptor) {
                found->second.m_variantType.swap(type.m_variantType);
                return;
            } else {
                error = true;
            }
        }
        if(error) {
            std::string error("incompatible type on line ");
            error.append(std::to_string(identifier.lineNumber));
            throw std::runtime_error(error);
        }
        // Add brand new instance
        if(identifier.lexeme != Lexeme::DOLLAR_STRING) {
            // Non-array handling
            m_cacheStack[0].emplace(identifier.raw, type);
        } 

        // Array handling
        else if(type.m_descriptor == TypeDescriptor::Int) {
            auto deduced = std::get<int64_t>(type.m_variantType);
            std::vector<int64_t> vec;
            vec.push_back(deduced);
            VariantType newv{vec};
            m_cacheStack[0].emplace(identifier.raw, Type{TypeDescriptor::Ints, newv});
        } else if (type.m_descriptor == TypeDescriptor::Real) {
            auto deduced = std::get<long double>(type.m_variantType);
            std::vector<long double> vec;
            vec.push_back(deduced);
            VariantType newv{vec};
            m_cacheStack[0].emplace(identifier.raw, Type{TypeDescriptor::Reals, newv});
        } else if (type.m_descriptor == TypeDescriptor::Bool) {
            auto deduced = std::get<bool>(type.m_variantType);
            std::vector<bool> vec;
            vec.push_back(deduced);
            VariantType newv{vec};
            m_cacheStack[0].emplace(identifier.raw, Type{TypeDescriptor::Bools, newv});
        } else if (type.m_descriptor == TypeDescriptor::String) {
            auto deduced = std::get<std::string>(type.m_variantType);
            std::vector<std::string> vec;
            vec.push_back(deduced);
            VariantType newv{vec};
            m_cacheStack[0].emplace(identifier.raw, Type{TypeDescriptor::Strings, newv});
        }
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
            std::string error("Index out of range on line ");
            error.append(std::to_string(identifier.lineNumber));
            throw std::runtime_error(error);
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