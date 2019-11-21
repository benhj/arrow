#include "LiteralRealExpression.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include <utility>

namespace jasl {

    LiteralRealExpression::LiteralRealExpression()
     : Expression()
     , m_realToken()
    {
    }

    std::shared_ptr<ExpressionEvaluator> LiteralRealExpression::getEvaluator() const
    {
        struct RealEvaluator : public ExpressionEvaluator {
            RealEvaluator(Token tok) : m_tok(std::move(tok))
            {
            }

            Type evaluate(Cache &) const override
            {
                return {TypeDescriptor::Real, {std::stold(m_tok.raw)}};
            }
          private:
            Token m_tok;
        };
        return std::make_shared<RealEvaluator>(m_realToken);
    }

    DecayType LiteralRealExpression::decayType() const
    {
        return DecayType::DECAYS_TO_REAL;
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