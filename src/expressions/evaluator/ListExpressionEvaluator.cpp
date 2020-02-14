/// (c) Ben Jones 2019

#include "ListExpressionEvaluator.hpp"
#include "expressions/ListExpression.hpp"
#include <utility>
#include <vector>

namespace arrow {
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
            if(evaluated.m_descriptor == TypeDescriptor::BracketlessList) {
                auto & innerParts = std::get<std::vector<Type>>(evaluated.m_variantType);
                for(auto const & inner : innerParts) {
                    elements.push_back(inner);
                }
            } else {
                elements.push_back(evaluated);
            }
        }
        return {TypeDescriptor::List, std::move(elements)};
    }
}