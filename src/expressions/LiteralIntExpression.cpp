#include "LiteralIntExpression.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include <utility>

namespace arrow {

    LiteralIntExpression::LiteralIntExpression(long const lineNumber) 
      : Expression(lineNumber)
      , m_intToken()
    {
    }

    std::shared_ptr<ExpressionEvaluator> LiteralIntExpression::getEvaluator() const
    {
        struct IntEvaluator : public ExpressionEvaluator {
            IntEvaluator(Token tok) : m_tok(std::move(tok))
            {
            }

            Type evaluate(Cache &) const override
            {
                return {TypeDescriptor::Int, {std::stoll(m_tok.raw)}};
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

    Token LiteralIntExpression::getIntToken() const
    {
        return m_intToken;
    }

    std::string LiteralIntExpression::toString() const
    {
        std::string str("\nLiteral int expression: ");
        return str.append(m_intToken.raw);
    }
}