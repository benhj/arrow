#include "HatHatStringExpression.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include "evaluator/IdentifierEvaluator.hpp"
#include <utility>

namespace jasl {

    HatHatStringExpression::HatHatStringExpression()
     : Expression()
     , m_hatHatStringToken()
    {
    }

    std::shared_ptr<ExpressionEvaluator> HatHatStringExpression::getEvaluator() const
    {
        struct HatHatStringEvaluator : public ExpressionEvaluator
        {
            explicit HatHatStringEvaluator(Token tok)
              : m_tok(std::move(tok))
            {
            }
            Type evaluate(Cache & cache) const override
            {
                auto t = IdentifierEvaluator(std::move(m_tok)).evaluate(cache);
                if(t.m_descriptor == TypeDescriptor::String) {
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

    DecayType HatHatStringExpression::decayType() const
    {
        return DecayType::DECAYS_TO_STRING;
    }

    HatHatStringExpression & HatHatStringExpression::withHatHatStringToken(Token hatHatStringToken)
    {
        m_hatHatStringToken = std::move(hatHatStringToken);
        return *this;
    }

    Token HatHatStringExpression::getHatHatStringToken() const
    {
        return m_hatHatStringToken;
    }

    std::string HatHatStringExpression::toString() const
    {
        std::string str("\n^^ string expression: ");
        return str.append(m_hatHatStringToken.raw);
    }
}