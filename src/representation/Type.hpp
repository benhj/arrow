/// (c) Ben Jones 2019 - present

#pragma once

#include "PodType.hpp"
#include "TypeDescriptor.hpp"
#include "VariantType.hpp"
#include <string>
#include <utility>

namespace arrow {

    namespace detail {
        struct TypeVisitor
        {
            template <class Var>
            std::string operator()(Var var) const
            {
                if constexpr(std::is_same_v<Var, std::string>) {
                    return var;
                } else if constexpr(std::is_same_v<Var, bool>) {
                    return var ? "true" : "false";
                } else if constexpr(std::is_same_v<Var, std::vector<Type>>) {
                    if(!var.empty()) {
                        std::string str("[");
                        auto it = std::begin(var);
                        for (; it != std::end(var) - 1; ++it) {
                            str.append(it->toString()).append(" ");
                        }
                        if(it != std::end(var)) {
                            str.append(it->toString());
                            str.append("]");
                        }
                        return str;
                    }
                    return "[]";
                } else if constexpr(std::is_same_v<Var, std::vector<int64_t>> ||
                                    std::is_same_v<Var, std::vector<real>> ||
                                    std::is_same_v<Var, std::vector<bool>> ||
                                    std::is_same_v<Var, std::vector<char>> ||
                                    std::is_same_v<Var, std::vector<std::string>>) {
                    std::string str("{");
                    auto it = std::begin(var);
                    for (; it != std::end(var) - 1; ++it) {
                        if constexpr(std::is_same_v<Var, std::vector<std::string>>) {
                            str.append("\"").append(*it).append("\", ");
                        } else if constexpr(std::is_same_v<Var, std::vector<char>>) {
                            str.push_back(*it);
                            str.append(", ");
                        } else {
                            str.append(std::to_string(*it)).append(", ");
                        }
                    }
                    if(it != std::end(var)) {
                        if constexpr(std::is_same_v<Var, std::vector<std::string>>) {
                            str.append("\"").append(*it).append("\"");
                        } else if constexpr(std::is_same_v<Var, std::vector<char>>) {
                            str.push_back(*it);
                        }  else {
                            str.append(std::to_string(*it));
                        }
                        str.append("}");
                    }
                    return str;
                } else if constexpr(std::is_same_v<Var, std::map<std::string, Type>>) {
                    std::string str;
                    for (auto const & element : var) {
                        str.append(element.first).append(": ");
                        str.append(element.second.toString());
                        str.append("\n");
                    }
                    return str;
                } else if constexpr(std::is_same_v<Var, PodType>) {
                    auto & data = var.m_namedData;
                    std::string str;
                    for (auto const & element : data) {
                        str.append(element.first).append(": ");
                        str.append(element.second.toString());
                        str.append("\n");
                    }
                    return str;
                } else if constexpr(std::is_same_v<Var, std::vector<PodType>>) {
                    // TODO
                    return "";
                } else {
                    return std::to_string(var);
                }
            }
        };
    }

    struct Type {
        Type(TypeDescriptor const descriptor,
             VariantType variantType)
          : m_descriptor(descriptor)
          , m_variantType(std::move(variantType))
        {
        }
        TypeDescriptor mutable m_descriptor;
        VariantType mutable m_variantType;

        bool listElementType() const
        {
            return m_descriptor == TypeDescriptor::String ||
                   m_descriptor == TypeDescriptor::ListWord;
        }

        bool operator==(Type const & other) const
        {
            return listElementType() == other.listElementType() &&
                   m_variantType == other.m_variantType;
        }

        std::string toString() const
        {
            return std::visit(detail::TypeVisitor(), m_variantType);
        }
    };
}