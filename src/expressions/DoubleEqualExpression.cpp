/// (c) Ben Jones 2019 - present

#include "DoubleEqualExpression.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include <utility>

namespace arrow {

    DoubleEqualExpression::DoubleEqualExpression(long const lineNumber) 
      : Expression(lineNumber)
      , m_token()
    {
    }

    std::shared_ptr<ExpressionEvaluator> DoubleEqualExpression::getEvaluator() const
    {
        struct DoubleEqualEvaluator : public ExpressionEvaluator
        {
            Type evaluate(Cache & cache) const override
            {
                return {TypeDescriptor::DoubleEqual, std::string("==")};
            }
        };
        return std::make_shared<DoubleEqualEvaluator>();
    }

    DoubleEqualExpression & DoubleEqualExpression::withToken(Token token)
    {
        m_token = std::move(token);
        return *this;
    }

    std::string DoubleEqualExpression::toString() const
    {
        return ""; // TODO
    }
}