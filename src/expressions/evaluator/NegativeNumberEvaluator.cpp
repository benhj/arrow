/// (c) Ben Jones 2020 - present

#include "NegativeNumberEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

namespace arrow {

    NegativeNumberEvaluator::NegativeNumberEvaluator(std::shared_ptr<ExpressionEvaluator> innerEvaluator,
                                                     long const lineNumber) 
      : m_innerEvaluator(std::move(innerEvaluator))
      , m_lineNumber(lineNumber)
    {
    }

    Type NegativeNumberEvaluator::evaluate(Environment & environment) const
    {

        auto evaluated = m_innerEvaluator->evaluate(environment);
        if(evaluated.m_descriptor == TypeDescriptor::Real) {
            auto const val = -(std::get<real>(evaluated.m_variantType));
            return {TypeDescriptor::Real, val};
        }
        if(evaluated.m_descriptor == TypeDescriptor::Int) {
            auto const val = -(std::get<int64_t>(evaluated.m_variantType));
            return {TypeDescriptor::Int, val};
        }
        throw LanguageException("Bad type ", m_lineNumber);
    }
}