/// (c) Ben Jones 2025 - present

#include "CosFunctionExpressionEvaluator.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <math.h>
#include <utility>

namespace arrow {

    CosFunctionExpressionEvaluator::CosFunctionExpressionEvaluator(CosFunctionExpression expression)
      : m_expression(std::move(expression))
    {
    }
    Type CosFunctionExpressionEvaluator::evaluate(Environment & environment) const
    {
        auto const expression = m_expression.getExpression();
        auto const col = expression->getEvaluator()->evaluate(environment);
        auto const expressionCollEval = std::get<std::vector<Type>>(col.m_variantType);
        if(expressionCollEval.empty()) {
            throw LanguageException("Expected argument", m_expression.getLineNumber());
        }
        auto const eval = expressionCollEval[0];
        if(eval.m_descriptor == TypeDescriptor::Int) {
            auto casted = std::get<int64_t>(eval.m_variantType);
            auto val = static_cast<int64_t>(cos(casted));
            return {TypeDescriptor::Int, val};
        } else if(eval.m_descriptor == TypeDescriptor::Real) {
            auto casted = std::get<real>(eval.m_variantType);
            auto val = static_cast<real>(cos(casted));
            return {TypeDescriptor::Real, val};
        } else {
            throw LanguageException("Bad type for cos", m_expression.getLineNumber());
        }
    }
}