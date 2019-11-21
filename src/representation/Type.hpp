#pragma once

#include "TypeDescriptor.hpp"
#include "VariantType.hpp"
#include <utility>

namespace jasl {
    struct Type {
        Type(TypeDescriptor const descriptor,
             VariantType variantType)
          : m_descriptor(descriptor)
          , m_variantType(std::move(variantType))
        {
        }
        TypeDescriptor const m_descriptor;
        VariantType mutable m_variantType;
    };
}