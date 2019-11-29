#include "BracedExpressionCollectionExpression.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include <utility>
#include <iostream>

namespace arrow {

    BracedExpressionCollectionExpression::BracedExpressionCollectionExpression()
     : Expression()
    {
    }

    std::shared_ptr<ExpressionEvaluator> BracedExpressionCollectionExpression::getEvaluator() const
    {
        struct BracedExpressionCollectionExpressionEvaluator : 
               public ExpressionEvaluator
        {
            BracedExpressionCollectionExpressionEvaluator(BracedExpressionCollectionExpression ece)
              : m_ece(std::move(ece))
            {
            }
            Type evaluate(Cache & cache) const override
            {
                auto expressions = m_ece.getExpressionCollection();
                std::vector<Type> bigEval;
                auto remType = TypeDescriptor::Nil;
                for(auto const & expression : expressions) {

                    auto evaluated = expression->getEvaluator()->evaluate(cache);
                    if(remType == TypeDescriptor::Nil) {
                        remType = evaluated.m_descriptor;
                    } 
                    // Incompatible types. All types in a {a, b, c} expression
                    // should be the same
                    else if(evaluated.m_descriptor != remType) {
                        std::string error("Type mismatch on line... (TODO)");
                        throw std::runtime_error(error);
                    }

                    bigEval.emplace_back(evaluated);
                }
                return {TypeDescriptor::ExpressionCollection, bigEval};
            }
          private:
            BracedExpressionCollectionExpression m_ece;
        };
        return std::make_shared<BracedExpressionCollectionExpressionEvaluator>(*this);
    }

    DecayType BracedExpressionCollectionExpression::decayType() const
    {
        return DecayType::DECAYS_TO_NONE;
    }

    void BracedExpressionCollectionExpression::addExpression(std::shared_ptr<Expression> expression)
    {
        m_expressions.emplace_back(std::move(expression));
    }

    std::vector<std::shared_ptr<Expression>> 
    BracedExpressionCollectionExpression::getExpressionCollection() const
    {
        return m_expressions;
    }

    std::string BracedExpressionCollectionExpression::toString() const
    {
        if(!m_expressions.empty()) {
            std::string str("\nBegin braced expression collection: ");
            for(auto const & exp : m_expressions) {
                str.append(exp->toString());
            }
            str.append("\nEnd braced expression collection.");
            return str;
        }
        return "";
    }
}