#include "QQStringExpression.hpp"
#include <utility>

namespace arrow {

    QQStringExpression::QQStringExpression(long const lineNumber) 
      : Expression(lineNumber)
      , m_qQStringToken()
    {
    }

    QQStringExpression & QQStringExpression::withQQStringToken(Token qQStringToken)
    {
        m_qQStringToken = std::move(qQStringToken);
        return *this;
    }

    Token QQStringExpression::getQQStringToken() const
    {
        return m_qQStringToken;
    }

    std::shared_ptr<ExpressionEvaluator> 
    QQStringExpression::getEvaluator() const
    {
        struct QQStringExpressionEvaluator : public ExpressionEvaluator
        {
            QQStringExpressionEvaluator(QQStringExpression expression)
              : m_expression(std::move(expression))
            {
            }

            Type evaluate(Cache &) const override
            {
                auto const token = m_expression.getQQStringToken();
                return {TypeDescriptor::QQString, token.raw};
            }

          private:
            QQStringExpression m_expression;
        };
        return std::make_shared<QQStringExpressionEvaluator>(*this);
    }

    std::string QQStringExpression::toString() const
    {
        std::string str("\n?? string expression: ");
        return str.append(m_qQStringToken.raw);
    }
}