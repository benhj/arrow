#include "ListExpression.hpp"
#include "evaluator/ListExpressionEvaluator.hpp"

namespace arrow {

    ListExpression::ListExpression(long const lineNumber) 
      : Expression(lineNumber)
     , m_parts{}
    {
    }

    std::shared_ptr<ExpressionEvaluator> ListExpression::getEvaluator() const
    {
        return std::make_shared<ListExpressionEvaluator>(*this);
    }

    void ListExpression::addPart(std::shared_ptr<Expression> expression)
    {
        m_parts.emplace_back(std::move(expression));
    }

    int ListExpression::getPartsCount() const
    {
        return m_parts.size();
    }

    std::vector<std::shared_ptr<Expression>> ListExpression::getParts() const
    {
        return m_parts;
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