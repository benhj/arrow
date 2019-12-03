#include "HatStringExpression.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include "evaluator/IdentifierEvaluator.hpp"
#include <utility>

namespace arrow {

    HatStringExpression::HatStringExpression(long const lineNumber) 
      : Expression(lineNumber)
      , m_hatStringToken()
    {
    }

    std::shared_ptr<ExpressionEvaluator> HatStringExpression::getEvaluator() const
    {
        struct HatStringEvaluator : public ExpressionEvaluator
        {
            explicit HatStringEvaluator(Token tok)
              : m_tok(std::move(tok))
            {
            }
            Type evaluate(Cache & cache) const override
            {
                return IdentifierEvaluator(std::move(m_tok)).evaluate(cache);
            }
          private:
            Token m_tok;
        };
        return std::make_shared<HatStringEvaluator>(m_hatStringToken);
    }

    HatStringExpression & HatStringExpression::withHatStringToken(Token hatStringToken)
    {
        m_hatStringToken = std::move(hatStringToken);
        return *this;
    }

    Token HatStringExpression::getHatStringToken() const
    {
        return m_hatStringToken;
    }

    std::string HatStringExpression::toString() const
    {
        std::string str("\n^ string expression: ");
        return str.append(m_hatStringToken.raw);
    }
}