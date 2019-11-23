#include "ExpressionCollectionExpression.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include <utility>
#include <iostream>

namespace arrow {

    ExpressionCollectionExpression::ExpressionCollectionExpression()
     : Expression()
    {
    }

    std::shared_ptr<ExpressionEvaluator> ExpressionCollectionExpression::getEvaluator() const
    {
        struct ExpressionCollectionExpressionEvaluator : 
               public ExpressionEvaluator
        {
            ExpressionCollectionExpressionEvaluator(ExpressionCollectionExpression ece)
              : m_ece(std::move(ece))
            {
            }
            Type evaluate(Cache & cache) const override
            {
                auto expressions = m_ece.getExpressionCollection();
                std::vector<Type> bigEval;
                for(auto const & expression : expressions) {
                    bigEval.emplace_back(expression->getEvaluator()->evaluate(cache));
                }
                return {TypeDescriptor::ExpressionCollection, bigEval};
            }
          private:
            ExpressionCollectionExpression m_ece;
        };
        return std::make_shared<ExpressionCollectionExpressionEvaluator>(*this);
    }

    DecayType ExpressionCollectionExpression::decayType() const
    {
        return DecayType::DECAYS_TO_NONE;
    }

    void ExpressionCollectionExpression::addExpression(std::shared_ptr<Expression> expression)
    {
        m_expressions.emplace_back(std::move(expression));
    }

    std::vector<std::shared_ptr<Expression>> 
    ExpressionCollectionExpression::getExpressionCollection() const
    {
        return m_expressions;
    }

    std::string ExpressionCollectionExpression::toString() const
    {
        if(!m_expressions.empty()) {
            std::string str("\nBegin expression collection: ");
            for(auto const & exp : m_expressions) {
                str.append(exp->toString());
            }
            str.append("\nEnd expression collection.");
            return str;
        }
        return "";
    }
}