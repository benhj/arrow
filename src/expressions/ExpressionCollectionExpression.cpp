#include "ExpressionCollectionExpression.hpp"
#include <utility>

namespace arrow {

    ExpressionCollectionExpression::ExpressionCollectionExpression()
     : Expression()
    {
    }

    std::shared_ptr<ExpressionEvaluator> ExpressionCollectionExpression::getEvaluator() const
    {
        return nullptr;
    }
/*
    Type ExpressionCollectionExpression::evaluate() const
    {
        return {TypeDescriptor::None, {false}};
    }
*/
    DecayType ExpressionCollectionExpression::decayType() const
    {
        return DecayType::DECAYS_TO_NONE;
    }

    void ExpressionCollectionExpression::addExpression(std::shared_ptr<Expression> expression)
    {
        m_expressions.emplace_back(std::move(expression));
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