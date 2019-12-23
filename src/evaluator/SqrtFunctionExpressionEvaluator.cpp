/// (c) Ben Jones 2019

#include "SqrtFunctionExpressionEvaluator.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <math.h>
#include <utility>

namespace arrow {

    SqrtFunctionExpressionEvaluator::SqrtFunctionExpressionEvaluator(SqrtFunctionExpression expression)
      : m_expression(std::move(expression))
    {
    }
    Type SqrtFunctionExpressionEvaluator::evaluate(Cache & cache) const
    {
        auto const eval = m_expression.getEvaluator()->evaluate(cache);
        if(eval.m_descriptor == TypeDescriptor::Int) {
            auto casted = std::get<int64_t>(eval.m_variantType);
            auto val = static_cast<long double>(::sqrt(casted));
            return {TypeDescriptor::Real, val};
        } else if(eval.m_descriptor == TypeDescriptor::Real) {
            auto casted = std::get<long double>(eval.m_variantType);
            auto val = static_cast<long double>(::sqrt(casted));
            return {TypeDescriptor::Real, val};
        } else {
            throw LanguageException("Bad type for sqrt", m_expression.getLineNumber());
        }
    }
}