/// (c) Ben Jones 2019 - present

#include "SqrtFunctionExpression.hpp"
#include "evaluator/ExpressionEvaluator.hpp"

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