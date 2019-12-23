/// (c) Ben Jones 2019 - present

#include "SqrtFunctionExpression.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <cmath>

namespace arrow {

    SqrtFunctionExpression::SqrtFunctionExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> SqrtFunctionExpression::getEvaluator() const
    {
        struct SqrtFunctionExpressionEvaluator : public ExpressionEvaluator
        {
            SqrtFunctionExpressionEvaluator(SqrtFunctionExpression expression)
              : m_expression(std::move(expression))
            {
            }

            Type evaluate(Cache & cache) const override
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

          private:
            SqrtFunctionExpression m_expression;
        };
        return std::make_shared<SqrtFunctionExpressionEvaluator>(*this);
    }

    SqrtFunctionExpression & 
    SqrtFunctionExpression::withExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
        return *this;
    }
    std::shared_ptr<Expression>
    SqrtFunctionExpression::getExpression() const
    {
        return m_expression;
    }

    std::string SqrtFunctionExpression::toString() const
    {
        return ""; // todo
    }
}