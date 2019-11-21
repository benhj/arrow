#include "LiteralStringExpression.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include <utility>

namespace jasl {

    LiteralStringExpression::LiteralStringExpression()
     : Expression()
     , m_stringToken()
    {
    }

    std::shared_ptr<ExpressionEvaluator> LiteralStringExpression::getEvaluator() const
    {
        struct StringEvaluator : public ExpressionEvaluator {
            StringEvaluator(Token tok) : m_tok(std::move(tok))
            {
            }

            Type evaluate(Cache &) const override
            {
                return {TypeDescriptor::String, m_tok.raw};
            }
          private:
            Token m_tok;
        };
        return std::make_shared<StringEvaluator>(m_stringToken);
    }

    DecayType LiteralStringExpression::decayType() const
    {
        return DecayType::DECAYS_TO_STRING;
    }

    LiteralStringExpression & LiteralStringExpression::withStringToken(Token stringToken)
    {
        m_stringToken = std::move(stringToken);
        return *this;
    }

    Token LiteralStringExpression::getStringToken() const
    {
        return m_stringToken;
    }

    std::string LiteralStringExpression::toString() const
    {
        std::string str("\nLiteral string expression: ");
        return str.append(m_stringToken.raw);
    }
}