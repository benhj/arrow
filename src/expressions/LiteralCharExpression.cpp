/// (c) Ben Jones 2019 - present

#include "LiteralCharExpression.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include <utility>

namespace arrow {

    LiteralCharExpression::LiteralCharExpression(long const lineNumber) 
      : Expression(lineNumber)
      , m_stringToken()
    {
    }

    std::shared_ptr<ExpressionEvaluator> LiteralCharExpression::getEvaluator() const
    {
        struct LiteralCharEvaluator final : public ExpressionEvaluator {
            LiteralCharEvaluator(Token tok) : m_tok(std::move(tok))
            {
            }

            Type evaluate(Environment &) const override
            {
                return {TypeDescriptor::Byte, m_tok.raw[0]};
            }
          private:
            Token m_tok;
        };
        return std::make_shared<LiteralCharEvaluator>(m_stringToken);
    }

    LiteralCharExpression & LiteralCharExpression::withStringToken(Token stringToken)
    {
        m_stringToken = std::move(stringToken);
        return *this;
    }

    Token const & LiteralCharExpression::getStringToken() const
    {
        return m_stringToken;
    }

    std::string LiteralCharExpression::toString() const
    {
        std::string str("\nLiteral char expression: ");
        return str.append(m_stringToken.raw);
    }
}