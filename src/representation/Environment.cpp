/// (c) Ben Jones 2019 - present

#include "Environment.hpp"
#include "parser/LanguageException.hpp"
#include <algorithm>

namespace arrow {

    namespace {
        template <typename T>
        void updateArray(VariantType & vt, Type element, int const index)
        {
            auto & casted = std::get<std::vector<T>>(vt);
            /*
            if(index >= casted.size()) {
                throw LanguageException("Index out of range", lineNumber);
            }*/
            if constexpr(std::is_same_v<T, Type>) {
                casted[index].m_variantType.swap(element.m_variantType);
            } else {
                auto const val = std::get<T>(element.m_variantType);
                casted[index] = val;
            }
        }

        template <typename T>
        std::pair<bool, std::vector<T>> tryErase(VariantType vt,
                                                 int const index)
        {
            try {
                auto casted = std::get<std::vector<T>>(vt);
                /*
                if(index >= casted.size()) {
                    throw LanguageException("Index out of range", lineNumber);
                }*/
                casted.erase(std::begin(casted) + index);
                return {true, casted};
            } catch (...) {
                return {false, std::vector<T>()};
            }
        }
    }

    Environment::Environment(std::ostream & ostr)
      : m_ostream(ostr)
      , m_environmentStack{}
      , m_functions()
      , m_pods()
      , m_programArguments()
      , m_exitState(false)
      , m_keyHandler(nullptr)
    {
        pushEnvironmentLayer();
    }

    std::ostream & Environment::getOutputStream()
    {
        return m_ostream;
    }

    Environment &
    Environment::withFunctions(std::map<std::string, std::shared_ptr<FunctionStatement>> funcs)
    {
        m_functions = std::move(funcs);
        return *this;
    }
    Environment &
    Environment::withPods(std::map<std::string, std::shared_ptr<PodStatement>> pods)
    {
        m_pods = std::move(pods);
        return *this;
    }

    Environment & Environment::withProgramArgs(std::vector<Type> progArgs)
    {
        m_programArguments = std::move(progArgs);
        return *this;
    }

    void Environment::initializeKeyHandler()
    {
        m_keyHandler = std::make_shared<KeyHandler>();
    }

    std::vector<Type> const & Environment::getProgramArgs() const
    {
        return m_programArguments;
    }

    bool Environment::getExitState() const
    {
        return m_exitState;
    }

    void Environment::setExitState()
    {
        m_exitState = true;
    }

    Environment::EnvironmentMap::iterator
    Environment::findAndRetrieveCached(std::string identifier) const
    {
        for (auto & layer : m_environmentStack) {
            auto found = layer.find(identifier);
            if(found != std::end(layer)) {
                return found;
            }
        }
        return EnvironmentMap::iterator();
    }

    Type & Environment::get(std::string identifier) const
    {
        auto found = findAndRetrieveCached(std::move(identifier));
        return found->second;
    }

    void Environment::add(std::string identifier, Type const type)
    {
        auto found = findAndRetrieveCached(identifier);
        if(found != EnvironmentMap::iterator()) {
            // Remove original instance of value
            found->second.m_variantType.swap(type.m_variantType);
            found->second.m_descriptor = type.m_descriptor;
            return;
        }
        // Add brand new instance
        m_environmentStack[0].emplace(identifier, type);
    }
    bool Environment::has(std::string identifier) const
    {
        auto found = findAndRetrieveCached(std::move(identifier));
        return found != EnvironmentMap::iterator();
    }
    void Environment::remove(std::string identifier) const
    {
        for (auto & layer : m_environmentStack) {
            auto found = layer.find(identifier);
            if(found != std::end(layer)) {
                layer.erase(found);
                return;
            }
        }
    }

    void Environment::pushBackContainerElement(std::string identifier, Type const type)
    {
        auto found = findAndRetrieveCached(std::move(identifier));
        
        if(found->second.m_descriptor == TypeDescriptor::Ints) {
            auto & casted = std::get<std::vector<int64_t>>(found->second.m_variantType);
            auto tval = std::get<int64_t>(type.m_variantType);
            casted.push_back(tval);
        } else if(found->second.m_descriptor == TypeDescriptor::Reals) {
            auto & casted = std::get<std::vector<real>>(found->second.m_variantType);
            auto tval = std::get<real>(type.m_variantType);
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
        } else if(found->second.m_descriptor == TypeDescriptor::Pods) {
            auto & casted = std::get<std::vector<PodType>>(found->second.m_variantType);
            auto tval = std::get<PodType>(type.m_variantType);
            casted.push_back(tval);
        } else if(found->second.m_descriptor == TypeDescriptor::List) {
            auto & casted = std::get<std::vector<Type>>(found->second.m_variantType);
            casted.push_back(type);
        } else if(found->second.m_descriptor == TypeDescriptor::Arrays) {
            auto & casted = std::get<std::vector<Type>>(found->second.m_variantType);
            casted.push_back(type);
        }  
    }

    void Environment::addElementToMapElementWhenList(std::string identifier, std::string key, const Type type)
    {
        auto found = findAndRetrieveCached(std::move(identifier));
        if(found->second.m_descriptor == TypeDescriptor::Map) {
            auto & casted = std::get<std::map<std::string, Type>>(found->second.m_variantType);
            auto foundInner = casted.find(key);
            if(foundInner->second.m_descriptor == TypeDescriptor::List) {
                auto & vec = std::get<std::vector<Type>>(foundInner->second.m_variantType);
                vec.push_back(type);
            } else {
                std::vector<Type> newvec;
                newvec.emplace_back(type);
                Type newType{TypeDescriptor::List, newvec};
                casted.emplace(key, std::move(newType));
            }
        }
    }

    void Environment::setElementInContainer(std::string identifier,
                                      int const index,
                                      Type const type)
    {
        auto found = findAndRetrieveCached(std::move(identifier));
        try {
            if(type.m_descriptor == TypeDescriptor::Int) {
                updateArray<int64_t>(found->second.m_variantType, type, index);
                return;
            } else if(type.m_descriptor == TypeDescriptor::Real) {
                updateArray<real>(found->second.m_variantType, type, index);
                return;
            } else if(type.m_descriptor == TypeDescriptor::String) {
                updateArray<std::string>(found->second.m_variantType, type, index);
                return;
            } else if(type.m_descriptor == TypeDescriptor::Byte) {
                updateArray<char>(found->second.m_variantType, type, index);
                return;
            } else if(type.m_descriptor == TypeDescriptor::Bool) {
                updateArray<bool>(found->second.m_variantType, type, index);
                return;
            } else if(type.m_descriptor == TypeDescriptor::Pod) {
                updateArray<PodType>(found->second.m_variantType, type, index);
                return;
            } 
        } catch (...) {
            // if casting threw, it must be because of bad type conversion
            // meaning we can instead try Type as the list element type.
        }
        updateArray<Type>(found->second.m_variantType, type, index);
    }

    void Environment::eraseElementInContainer(std::string identifier,
                                        int const index)
    {
        auto found = findAndRetrieveCached(std::move(identifier));
        {
            auto result = tryErase<int64_t>(found->second.m_variantType, index);
            if(result.first) {
                found->second.m_variantType = result.second;
                return;
            }
        }
        {
            auto result = tryErase<real>(found->second.m_variantType, index);
            if(result.first) {
                found->second.m_variantType = result.second;
                return;
            }
        }
        {
            auto result = tryErase<bool>(found->second.m_variantType, index);
            if(result.first) {
                found->second.m_variantType = result.second;
                return;
            }
        }
        {
            auto result = tryErase<char>(found->second.m_variantType, index);
            if(result.first) {
                found->second.m_variantType = result.second;
                return;
            }
        }
        {
            auto result = tryErase<std::string>(found->second.m_variantType, index);
            if(result.first) {
                found->second.m_variantType = result.second;
                return;
            }
        }
        {
            auto result = tryErase<PodType>(found->second.m_variantType, index);
            if(result.first) {
                found->second.m_variantType = result.second;
                return;
            }
        }
        {
            auto result = tryErase<Type>(found->second.m_variantType, index);
            if(result.first) {
                found->second.m_variantType = result.second;
                return;
            }
        }
    }

    void Environment::pushEnvironmentLayer()
    {
        m_environmentStack.emplace_front(EnvironmentMap());
    }
    void Environment::popEnvironmentLayer()
    {
        m_environmentStack.pop_front();
    }
    void Environment::addFunctionStatement(std::string identifier,
                                           std::shared_ptr<FunctionStatement> func)
    {
        m_functions.emplace(std::move(identifier), std::move(func));
    }
    void Environment::addPodStatement(std::string identifier,
                                      std::shared_ptr<PodStatement> pod)
    {
        m_pods.emplace(std::move(identifier), std::move(pod));
    }

    std::shared_ptr<FunctionStatement>
    Environment::getFunction(std::string identifier) const
    {
        auto found = std::find_if(std::begin(m_functions),
                                  std::end(m_functions),
                                  [identifier{std::move(identifier)}]
                                  (auto const & p) {
            return p.first == identifier;
        });

        if(found != std::end(m_functions)) {
            return found->second;
        }
        return nullptr;
    }

    std::shared_ptr<PodStatement> Environment::getPod(std::string identifier) const
    {
        auto found = std::find_if(std::begin(m_pods),
                                  std::end(m_pods),
                                  [identifier{std::move(identifier)}]
                                  (auto const & p) {
            return p.first == identifier;
        });

        if(found != std::end(m_pods)) {
            return found->second;
        }
        return nullptr;
    }

    std::map<std::string, std::shared_ptr<FunctionStatement>> const &
    Environment::getFunctions() const
    {
        return m_functions;
    }

    std::map<std::string, std::shared_ptr<PodStatement>> const &
    Environment::getPods() const
    {
        return m_pods;
    }

    void Environment::pushProgramArgument(Type arg)
    {
        m_programArguments.push_back(std::move(arg));
    }

    Type Environment::getProgramArgument(int64_t const index) const
    {
        return m_programArguments.at(index);
    }

    std::optional<char> Environment::retrieveKeyPress()
    {
        if(m_keyHandler) {
            if(m_keyHandler->was_any_key_pressed()) {
                return m_keyHandler->get_last_key();
            }
        }
        return std::nullopt;
    }

}