/// (c) Ben Jones 2025 - present

#include "Atan2FunctionExpressionEvaluator.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <math.h>
#include <utility>

namespace arrow {

    Atan2FunctionExpressionEvaluator::Atan2FunctionExpressionEvaluator(Atan2FunctionExpression expression)
      : m_expression(std::move(expression))
    {
    }
    Type Atan2FunctionExpressionEvaluator::evaluate(Environment & environment) const
    {
        auto const expression = m_expression.getExpression();
        auto const col = expression->getEvaluator()->evaluate(environment);
        auto const expressionCollEval = std::get<std::vector<Type>>(col.m_variantType);
        if(expressionCollEval.size() < 2) {
            throw LanguageException("Too few arguments", m_expression.getLineNumber());
        }
        auto const eval0 = expressionCollEval[0];
        auto const eval1 = expressionCollEval[1];
        if(eval0.m_descriptor != TypeDescriptor::Real ||
           eval1.m_descriptor != TypeDescriptor::Real) {
            throw LanguageException("Bad type for atan2", m_expression.getLineNumber());
        }
        auto casted0 = std::get<real>(eval0.m_variantType);
        auto casted1 = std::get<real>(eval1.m_variantType);
        auto val = static_cast<real>(atan2(casted0, casted1));
        return {TypeDescriptor::Real, val};
    }
}