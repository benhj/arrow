/// (c) Ben Jones 2019 - present

#pragma once

#include "Type.hpp"
#include "TypeDescriptor.hpp"
#include "statements/FunctionStatement.hpp"
#include <deque>
#include <map>
#include <memory>
#include <optional>
#include <mutex>
#include <string>

namespace arrow {

    class Environment
    {
      public:
        using EnvironmentMap = std::map<std::string, Type>;
        Environment();

        Environment & withFunctions(std::map<std::string, std::shared_ptr<FunctionStatement>>);
        Environment & withProgramArgs(std::vector<Type>);
        std::vector<Type> getProgramArgs() const;

        Type get(std::string identifier) const;
        void add(std::string identifier, Type const type);
        bool has(std::string identifier) const;
        void remove(std::string identifier) const;
        void pushEnvironmentLayer();
        void popEnvironmentLayer();
        void pushProgramArgument(Type arg);
        Type getProgramArgument(int64_t const index) const;

        void setElementInContainer(std::string identifier,
                                   int const index,
                                   Type const type);

        void eraseElementInContainer(std::string identifier,
                                     int const index);

        void pushBackContainerElement(std::string identifier, Type const type);

        void addFunctionStatement(std::string identifier,
                                  std::shared_ptr<FunctionStatement>);

        std::shared_ptr<FunctionStatement> getFunction(std::string identifier) const;
        std::map<std::string, std::shared_ptr<FunctionStatement>> getFunctions() const;

        EnvironmentMap::iterator findAndRetrieveCached(std::string identifier) const;

        bool getExitState() const;
        void setExitState();

      private:

        std::deque<EnvironmentMap> mutable m_environmentStack;
        std::map<std::string, std::shared_ptr<FunctionStatement>> m_functions;
        std::vector<Type> mutable m_programArguments;
        std::mutex mutable m_mutex;

        // When a program is exiting, this boolean will be true
        bool m_exitState;
    };
}