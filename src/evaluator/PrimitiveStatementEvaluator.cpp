#include "PrimitiveStatementEvaluator.hpp"
#include "ExpressionEvaluator.hpp"
#include <utility>

namespace jasl {

    namespace {
        bool decaysToInt(TypeDescriptor const descriptor)
        {
            return descriptor == TypeDescriptor::Int ||
                   descriptor == TypeDescriptor::Real;
        }
    }

    PrimitiveStatementEvaluator::PrimitiveStatementEvaluator(PrimitiveStatement statement)
      : m_statement(std::move(statement))
    {
    }
    void PrimitiveStatementEvaluator::evaluate(Cache & cache) const
    {
        auto const keyword = m_statement.getToken().raw;
        auto const expression = m_statement.getExpression();
        auto evaluated = expression->getEvaluator()->evaluate(cache);
        auto const identifier = m_statement.getIdentifier().raw;
        auto const typeDescriptor = evaluated.m_descriptor;
        if(keyword == "int" && decaysToInt(typeDescriptor)) {
            if(typeDescriptor == TypeDescriptor::Real) {
                auto const val = std::get<long double>(evaluated.m_variantType);
                auto const casted = static_cast<int64_t>(val);
                VariantType newType(casted);
                evaluated.m_variantType.swap(newType);
                evaluated.m_descriptor = TypeDescriptor::Int;
            }
            cache.add(identifier, evaluated);
        } else if(keyword == "real" && decaysToInt(typeDescriptor)) {
            if(typeDescriptor == TypeDescriptor::Int) {
                auto const val = std::get<int64_t>(evaluated.m_variantType);
                auto const casted = static_cast<long double>(val);
                VariantType newType(casted);
                evaluated.m_variantType.swap(newType);
                evaluated.m_descriptor = TypeDescriptor::Real;
            }
            cache.add(identifier, evaluated);
        } else if(keyword == "bool" && typeDescriptor == TypeDescriptor::Bool) {
            cache.add(identifier, evaluated);
        } else if(keyword == "string" && typeDescriptor == TypeDescriptor::String) {
            cache.add(identifier, evaluated);
        } else if(keyword == "list" && typeDescriptor == TypeDescriptor::List) {
            cache.add(identifier, evaluated);
        } else {
            throw std::runtime_error(std::string("Couldn't evaluate ").append(identifier));
        }
    }
}