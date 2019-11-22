#pragma once

#include "TypeDescriptor.hpp"
#include "VariantType.hpp"
#include <string>
#include <utility>

namespace jasl {

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
                    std::string str("[");
                    /*
                    for (auto const & v : var) {
                        str.append(v).append(" ");
                    }*/
                    str.append("]");
                    return str;
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

        std::string toString() const
        {
            return std::visit(detail::TypeVisitor(), m_variantType);
        }
    };
}