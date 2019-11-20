#include "GroupedExpression.hpp"

namespace jasl {

    GroupedExpression::GroupedExpression() 
    : Expression()
    , m_expression{nullptr}
    {
    }

    std::shared_ptr<Evaluator> GroupedExpression::getEvaluator() const
    {
        return nullptr;
    }

    Type GroupedExpression::evaluate() const
    {
        return {TypeDescriptor::None, {false}};
    }

    DecayType GroupedExpression::decayType() const
    {
        return DecayType::DECAYS_TO_NONE;
    }

    GroupedExpression & GroupedExpression::withExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
        return *this;
    }
    
    std::shared_ptr<Expression> GroupedExpression::getExpression() const
    {
        return m_expression;
    }
    std::string GroupedExpression::toString() const
    {
        std::string str("\nBegin grouped expression");
        str.append(m_expression->toString());
        str.append("\nEnd grouped expression");
        return str;
    }
}