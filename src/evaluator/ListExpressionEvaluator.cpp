#include "ListExpressionEvaluator.hpp"
#include "expressions/ListExpression.hpp"
#include <utility>
#include <vector>

namespace jasl {
    ListExpressionEvaluator::ListExpressionEvaluator(ListExpression listExpression)
      : m_listExpression(std::move(listExpression))
    {
    }
    Type ListExpressionEvaluator::evaluate(Cache & cache) const 
    {
        std::vector<Type> elements;
        auto const parts = m_listExpression.getParts();
        for(auto const & part : parts) {
            auto const evaluated = part->getEvaluator()->evaluate(cache);
            elements.push_back(evaluated);
        }
        return {TypeDescriptor::List, std::move(elements)};
    }
}