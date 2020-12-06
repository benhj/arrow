/// (c) Ben Jones 2019 - present

#pragma once

#include "Type.hpp"
#include "TypeDescriptor.hpp"
#include "statements/FunctionStatement.hpp"
#include "statements/PodStatement.hpp"
#include <deque>
#include <map>
#include <memory>
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

        /// Add function table
        Environment & withFunctions(std::map<std::string, std::shared_ptr<FunctionStatement>>);

        /// Add POD (plain old data) table -- note rudimentary OO support
        Environment & withPods(std::map<std::string, std::shared_ptr<PodStatement>>);

        /// Add program argument lookup table
        Environment & withProgramArgs(std::vector<Type>);

        /// Retrieve program argument table
        std::vector<Type> const & getProgramArgs() const;

        /// Retrieve a cached type with given identifier
        Type & get(std::string identifier) const;

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

        /// Add a program argument to the program arg container
        void pushProgramArgument(Type arg);

        /// Retrieve a program arg. at given index
        Type getProgramArgument(int64_t const index) const;

        /// Sets an element in a cached container type
        void setElementInContainer(std::string identifier,
                                   int const index,
                                   Type const type);

        /// Erases an element in a cached container type
        void eraseElementInContainer(std::string identifier,
                                     int const index);

        /// Adds an element to the end of a container type
        void pushBackContainerElement(std::string identifier, Type const type);

        /// Adds an element to a list when keyed element of a map
        /// is a list. This permits dollar syntax, as in:
        /// "hello" -> $map:key;
        void addElementToMapElementWhenList(std::string identifier, std::string key, Type const type);

        /// Adds a function statement to the function lookup table
        void addFunctionStatement(std::string identifier,
                                  std::shared_ptr<FunctionStatement>);

        /// Retrieves a function from the function lookup map
        std::shared_ptr<FunctionStatement> getFunction(std::string identifier) const;

        /// Adds a pod statement to the pod lookup table
        void addPodStatement(std::string identifier,
                             std::shared_ptr<PodStatement>);

        /// Retrieves a pod from the pod lookup map
        std::shared_ptr<PodStatement> getPod(std::string identifier) const;

        /// Retrieves the function lookup table
        std::map<std::string, std::shared_ptr<FunctionStatement>> const & getFunctions() const;

        /// Retrieves the pod lookup table
        std::map<std::string, std::shared_ptr<PodStatement>> const & getPods() const;

        /// Returns an iterator to a cached element
        EnvironmentMap::iterator findAndRetrieveCached(std::string identifier) const;

        /// Retrieves exit state (note, set when 'exit' is called from within program)
        bool getExitState() const;

        /// Sets the program exit state to true
        void setExitState();

      private:
        /// For outputting program to stream (e.g. std::out, or some gui output stream).
        std::ostream & m_ostream;

        /// A stack of type caches (rename?)
        std::deque<EnvironmentMap> mutable m_environmentStack;

        /// Function lookup table (note, generated during parsing)
        std::map<std::string, std::shared_ptr<FunctionStatement>> m_functions;

        /// POD lookup table (note, generated during parsing)
        std::map<std::string, std::shared_ptr<PodStatement>> m_pods;

        /// Program arguments (note, generated from main at program start)
        std::vector<Type> mutable m_programArguments;

        /// Added when experimenting with async command
        std::mutex mutable m_mutex;

        // When a program is exiting, this boolean will be true
        bool m_exitState;
    };
}