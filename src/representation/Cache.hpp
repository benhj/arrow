#pragma once

#include "Type.hpp"
#include "TypeDescriptor.hpp"
#include "lexer/Token.hpp"
#include <deque>
#include <map>
#include <string>

namespace arrow {

    class Cache
    {
      public:
        using CacheMap = std::map<std::string, Type>;
        Cache();
        Type get(Token identifier) const;
        void add(Token identifier, Type const type);
        bool has(Token identifier) const;
        void remove(Token identifier) const;
        void pushCacheLayer();
        void popCacheLayer();
        void pushProgramArgument(Type arg);
        Type getProgramArgument(int64_t const index) const;

        void setElementInContainer(Token identifier,
                                   int const index,
                                   Type const type);

        static void pushReturnValue(Type t);
        static Type getAndPopReturnValue();

      private:

        CacheMap::iterator findAndRetrieveCached(std::string identifier) const;
        std::deque<CacheMap> mutable m_cacheStack;
        std::vector<Type> mutable m_programArguments;
        static std::deque<Type> m_returnStack;

    };
}