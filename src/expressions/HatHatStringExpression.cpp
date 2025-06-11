/// (c) Ben Jones 2019 - present

#include "HatHatStringExpression.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include "evaluator/IdentifierEvaluator.hpp"
#include <utility>

namespace arrow {

    HatHatStringExpression::HatHatStringExpression(long const lineNumber) 
      : Expression(lineNumber)
      , m_hatHatStringToken()
    {
    }

    std::shared_ptr<ExpressionEvaluator> HatHatStringExpression::getEvaluator() const
    {
        struct HatHatStringEvaluator final : public ExpressionEvaluator
        {
            explicit HatHatStringEvaluator(Token tok)
              : m_tok(std::move(tok))
            {
            }
            Type evaluate(Environment & environment) const override
            {
                auto t = IdentifierEvaluator(std::move(m_tok)).evaluate(environment);
                if(t.m_descriptor == TypeDescriptor::String) {
                    t.m_descriptor = TypeDescriptor::ListWord;
                    return t;
                } else {
                    // Must be an embedded list
                    t.m_descriptor = TypeDescriptor::BracketlessList;
                    return t;
                }
            }
          private:
            Token m_tok;
        };
        return std::make_shared<HatHatStringEvaluator>(m_hatHatStringToken);
    }

    HatHatStringExpression & HatHatStringExpression::withHatHatStringToken(Token hatHatStringToken)
    {
        m_hatHatStringToken = std::move(hatHatStringToken);
        return *this;
    }

    Token const & HatHatStringExpression::getHatHatStringToken() const
    {
        return m_hatHatStringToken;
    }

    std::string HatHatStringExpression::toString() const
    {
        std::string str("\n^^ string expression: ");
        return str.append(m_hatHatStringToken.raw);
    }
}