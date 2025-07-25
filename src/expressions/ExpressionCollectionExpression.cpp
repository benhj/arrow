/// (c) Ben Jones 2019 - present

#include "ExpressionCollectionExpression.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include <utility>
#include <iostream>

namespace arrow {

    ExpressionCollectionExpression::ExpressionCollectionExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> ExpressionCollectionExpression::getEvaluator() const
    {
        struct ExpressionCollectionExpressionEvaluator final : 
               public ExpressionEvaluator
        {
            ExpressionCollectionExpressionEvaluator(ExpressionCollectionExpression ece)
              : m_ece(std::move(ece))
            {
            }
            Type evaluate(Environment & environment) const override
            {
                auto expressions = m_ece.getExpressionCollection();
                std::vector<Type> bigEval;
                for(auto const & expression : expressions) {
                    bigEval.emplace_back(expression->getEvaluator()->evaluate(environment));
                }
                return {TypeDescriptor::ExpressionCollection, bigEval};
            }
          private:
            ExpressionCollectionExpression m_ece;
        };
        return std::make_shared<ExpressionCollectionExpressionEvaluator>(*this);
    }

    void ExpressionCollectionExpression::addExpression(std::shared_ptr<Expression> expression)
    {
        m_expressions.emplace_back(std::move(expression));
    }

    std::vector<std::shared_ptr<Expression>> const &
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