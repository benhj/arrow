/// (c) Ben Jones 2019 - present

#include "QStringExpression.hpp"
#include <utility>

namespace arrow {

    QStringExpression::QStringExpression(long const lineNumber) 
      : Expression(lineNumber)
      , m_qStringToken()
    {
    }

    QStringExpression & QStringExpression::withQStringToken(Token qStringToken)
    {
        m_qStringToken = std::move(qStringToken);
        return *this;
    }

    Token const & QStringExpression::getQStringToken() const
    {
        return m_qStringToken;
    }

    std::shared_ptr<ExpressionEvaluator> 
    QStringExpression::getEvaluator() const
    {
        struct QStringExpressionEvaluator : public ExpressionEvaluator
        {
            QStringExpressionEvaluator(QStringExpression expression)
              : m_expression(std::move(expression))
            {
            }

            Type evaluate(Environment &) const override
            {
                auto const token = m_expression.getQStringToken();
                return {TypeDescriptor::QString, token.raw};
            }

          private:
            QStringExpression m_expression;
        };
        return std::make_shared<QStringExpressionEvaluator>(*this);
    }

    std::string QStringExpression::toString() const
    {
        std::string str("\n? string expression: ");
        return str.append(m_qStringToken.raw);
    }
}