#include "IdentifierExpression.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include <utility>

namespace arrow {

    IdentifierExpression::IdentifierExpression()
     : Expression()
     , m_identifier()
    {
    }

    std::shared_ptr<ExpressionEvaluator> 
    IdentifierExpression::getEvaluator() const
    {
        struct IdentifierEvaluator : public ExpressionEvaluator {
            IdentifierEvaluator(Token tok) : m_tok(std::move(tok))
            {
            }

            Type evaluate(Cache & cache) const override
            {
                if(m_tok.raw == "true") {
                    return {TypeDescriptor::Bool, true};
                } else if(m_tok.raw == "false") {
                    return {TypeDescriptor::Bool, false};
                } else if(!cache.has(m_tok.raw)) {
                    std::string error("Value for identifier not found on line ");
                    error.append(std::to_string(m_tok.lineNumber));
                    throw std::runtime_error(error);
                }
                return cache.get(m_tok.raw);
            }
          private:
            Token m_tok;
        };
        return std::make_shared<IdentifierEvaluator>(m_identifier);
    }

    DecayType IdentifierExpression::decayType() const
    {
        return DecayType::DECAYS_TO_STRING;
    }

    IdentifierExpression & IdentifierExpression::withIdentifierToken(Token identifier)
    {
        m_identifier = std::move(identifier);
        return *this;
    }

    Token IdentifierExpression::getIdentifierToken() const
    {
        return  m_identifier;
    }

    std::string IdentifierExpression::toString() const
    {
        std::string str("\nIdentifier expression: ");
        return str.append(m_identifier.raw);
    }
}