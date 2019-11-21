#include "ListExpression.hpp"

namespace jasl {

    ListExpression::ListExpression() 
    : Expression()
    , m_parts{}
    {
    }

    std::shared_ptr<ExpressionEvaluator> ListExpression::getEvaluator() const
    {
        return nullptr;
    }
/*
    Type ListExpression::evaluate() const
    {
        return {TypeDescriptor::List, {false}};
    }
*/
    DecayType ListExpression::decayType() const
    {
        return DecayType::DECAYS_TO_LIST;
    }

    void ListExpression::addPart(std::shared_ptr<Expression> expression)
    {
        m_parts.emplace_back(std::move(expression));
    }

    int ListExpression::getPartsCount() const
    {
        return m_parts.size();
    }
    
    std::string ListExpression::toString() const
    {
        std::string str("\nBegin list expression");
        for(auto const & part : m_parts) {
            str.append("\n");
            str.append(part->toString());
        }
        str.append("\nEnd list expression");
        return str;
    }
}