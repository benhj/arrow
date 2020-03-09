/// (c) Ben Jones 2019 - present

#include "LiteralStringExpression.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include <utility>

namespace arrow {

    LiteralStringExpression::LiteralStringExpression(long const lineNumber) 
      : Expression(lineNumber)
      , m_stringToken()
    {
    }

    std::shared_ptr<ExpressionEvaluator> LiteralStringExpression::getEvaluator() const
    {
        struct StringEvaluator : public ExpressionEvaluator {
            StringEvaluator(Token tok) : m_tok(std::move(tok))
            {
            }

            Type evaluate(Environment &) const override
            {
                return {TypeDescriptor::String, m_tok.raw};
            }
          private:
            Token m_tok;
        };
        return std::make_shared<StringEvaluator>(m_stringToken);
    }

    LiteralStringExpression & LiteralStringExpression::withStringToken(Token stringToken)
    {
        m_stringToken = std::move(stringToken);
        return *this;
    }

    Token const & LiteralStringExpression::getStringToken() const
    {
        return m_stringToken;
    }

    std::string LiteralStringExpression::toString() const
    {
        std::string str("\nLiteral string expression: ");
        return str.append(m_stringToken.raw);
    }
}