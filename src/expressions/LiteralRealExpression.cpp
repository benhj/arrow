/// (c) Ben Jones 2019 - present

#include "LiteralRealExpression.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include "utility/from_string.hpp"
#include <utility>

namespace arrow {

    LiteralRealExpression::LiteralRealExpression(long const lineNumber) 
      : Expression(lineNumber)
      , m_realToken()
    {
    }

    std::shared_ptr<ExpressionEvaluator> LiteralRealExpression::getEvaluator() const
    {
        struct RealEvaluator : public ExpressionEvaluator {
            RealEvaluator(Token tok) : m_tok(std::move(tok))
            {
            }

            Type evaluate(Environment &) const override
            {
                return {TypeDescriptor::Real, {from_string<real>(m_tok.raw)}};
            }
          private:
            Token m_tok;
        };
        return std::make_shared<RealEvaluator>(m_realToken);
    }

    LiteralRealExpression & LiteralRealExpression::withRealToken(Token realToken)
    {
        m_realToken = std::move(realToken);
        return *this;
    }

    Token LiteralRealExpression::getRealToken() const
    {
        return m_realToken;
    }

    std::string LiteralRealExpression::toString() const
    {
        std::string str("\nLiteral real expression: ");
        return str.append(m_realToken.raw);
    }
}