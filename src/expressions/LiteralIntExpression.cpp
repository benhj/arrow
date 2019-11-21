#include "LiteralIntExpression.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include <utility>

namespace jasl {

    LiteralIntExpression::LiteralIntExpression()
     : Expression()
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

    DecayType LiteralIntExpression::decayType() const
    {
        return DecayType::DECAYS_TO_INT;
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