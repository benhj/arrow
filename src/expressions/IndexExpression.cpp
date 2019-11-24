#include "IndexExpression.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include <utility>

namespace arrow {

    IndexExpression::IndexExpression()
     : Expression()
    {
    }

    std::shared_ptr<ExpressionEvaluator> 
    IndexExpression::getEvaluator() const
    {
        struct IndexExpressionEvaluator : public ExpressionEvaluator {
            IndexExpressionEvaluator(Token tok, std::shared_ptr<Expression> expression) 
              : m_tok(std::move(tok))
              , m_expression(std::move(expression))
            {
            }

            Type evaluate(Cache & cache) const override
            {
                if(!cache.has(m_tok.raw)) {
                    std::string error("Value for identifier not found on line ");
                    error.append(std::to_string(m_tok.lineNumber));
                    throw std::runtime_error(error);
                }

                auto type = cache.get(m_tok.raw);
                if(type.m_descriptor != TypeDescriptor::List &&
                    type.m_descriptor != TypeDescriptor::ExpressionCollection &&
                    type.m_descriptor != TypeDescriptor::Ints &&
                    type.m_descriptor != TypeDescriptor::Reals &&
                    type.m_descriptor != TypeDescriptor::Bools &&
                    type.m_descriptor != TypeDescriptor::Strings) {
                    std::string error("Incompatiable type for index on line ");
                    error.append(std::to_string(m_tok.lineNumber));
                    throw std::runtime_error(error);
                }
                auto container = std::get<std::vector<Type>>(type.m_variantType);
                auto index = m_expression->getEvaluator()->evaluate(cache);
                auto deduced = std::get<int64_t>(index.m_variantType);
                if(deduced >= container.size()) {
                    std::string error("Index too large on line ");
                    error.append(std::to_string(m_tok.lineNumber));
                    throw std::runtime_error(error);
                }
                return container[deduced];
            }
          private:
            Token m_tok;
            std::shared_ptr<Expression> m_expression;
        };
        return std::make_shared<IndexExpressionEvaluator>(m_identifier, m_indexExpression);
    }

    DecayType IndexExpression::decayType() const
    {
        return DecayType::DECAYS_TO_STRING;
    }

    IndexExpression & IndexExpression::withIdentifierToken(Token identifier)
    {
        m_identifier = std::move(identifier);
        return *this;
    }

    IndexExpression & 
    IndexExpression::withIndexExpression(std::shared_ptr<Expression> indexExp)
    {
        m_indexExpression = std::move(indexExp);
        return *this;
    }

    Token IndexExpression::getIdentifierToken() const
    {
        return  m_identifier;
    }

    std::shared_ptr<Expression> IndexExpression::getIndexExpression() const
    {
        return m_indexExpression;
    }

    std::string IndexExpression::toString() const
    {
        /*
        std::string str("\nIndex expression: ");
        return str.append(m_identifier.raw);
        */
        return "";
    }
}