/// (c) Ben Jones 2019 - present

#include "LiteralIntExpression.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include "utility/from_string.hpp"
#include <utility>

namespace arrow {

    LiteralIntExpression::LiteralIntExpression(long const lineNumber) 
      : Expression(lineNumber)
      , m_intToken()
    {
    }

    std::shared_ptr<ExpressionEvaluator> LiteralIntExpression::getEvaluator() const
    {
        struct IntEvaluator final : public ExpressionEvaluator {
            IntEvaluator(Token tok) : m_tok(std::move(tok))
            {
            }

            Type evaluate(Environment &) const override
            {
                return {TypeDescriptor::Int, {from_string<int64_t>(m_tok.raw)}};
            }
          private:
            Token m_tok;
        };
        return std::make_shared<IntEvaluator>(m_intToken);
    }

    LiteralIntExpression & LiteralIntExpression::withIntToken(Token intToken)
    {
        m_intToken = std::move(intToken);
        return *this;
    }

    Token const & LiteralIntExpression::getIntToken() const
    {
        return m_intToken;
    }

    std::string LiteralIntExpression::toString() const
    {
        std::string str("\nLiteral int expression: ");
        return str.append(m_intToken.raw);
    }
}