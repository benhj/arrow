/// (c) Ben Jones 2019 - present

#pragma once

#include "Type.hpp"
#include "TypeDescriptor.hpp"
#include <deque>
#include <map>
#include <optional>
#include <ostream>
#include <mutex>
#include <string>

namespace arrow {

    class Environment
    {
      public:
        using EnvironmentMap = std::map<std::string, Type>;
        explicit Environment(std::ostream & os);

        /// Retrive the output stream used for writing output
        /// (for example, std::out)
        std::ostream & getOutputStream();

        /// Retrieve a cached type with given identifier
        Type get(std::string identifier) const;

        /// Cache a type with given identifier
        void add(std::string identifier, Type const type);

        /// Queries if the cache contains a type given identifier
        bool has(std::string identifier) const;

        /// Removes an item from the types cache
        void remove(std::string identifier) const;

        /// Adds a 'layer' to the environment -- used during
        /// scoping. A new scope has a new environment layer
        /// that is pushed to when caching new items.
        void pushEnvironmentLayer();

        /// Popping an environment layer when exiting a scope.
        void popEnvironmentLayer();
        void pushProgramArgument(Type arg);
        Type getProgramArgument(int64_t const index) const;

        void setElementInContainer(std::string identifier,
                                   int const index,
                                   Type const type);

        void eraseElementInContainer(std::string identifier,
                                     int const index);

        void pushBackContainerElement(std::string identifier, Type const type);

        static void pushReturnValue(Type t);
        static Type getAndPopReturnValue();

        EnvironmentMap::iterator findAndRetrieveCached(std::string identifier) const;

      private:
        std::ostream & m_ostream;
        std::deque<EnvironmentMap> mutable m_environmentStack;
        std::vector<Type> mutable m_programArguments;
        static std::deque<Type> m_returnStack;
        std::mutex mutable m_mutex;
    };
}