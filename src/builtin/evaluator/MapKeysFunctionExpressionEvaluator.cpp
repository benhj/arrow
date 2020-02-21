/// (c) Ben Jones 2020 - present

#include "MapKeysFunctionExpressionEvaluator.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <math.h>
#include <utility>

namespace arrow {

    MapKeysFunctionExpressionEvaluator::MapKeysFunctionExpressionEvaluator(MapKeysFunctionExpression expression)
      : m_expression(std::move(expression))
    {
    }
    Type MapKeysFunctionExpressionEvaluator::evaluate(Environment & environment) const
    {
        auto const expression = m_expression.getExpression();
        auto const col = expression->getEvaluator()->evaluate(environment);
        auto & expressionCollEval = std::get<std::vector<Type>>(col.m_variantType);
        if(expressionCollEval.empty()) {
            throw LanguageException("Expected argument", m_expression.getLineNumber());
        }
        auto const mapEval = expressionCollEval[0];
        if(mapEval.m_descriptor != TypeDescriptor::Map) {
            throw LanguageException("Not a map", m_expression.getLineNumber());
        }

        auto & themap = std::get<std::map<std::string, Type>>(mapEval.m_variantType);
        std::vector<std::string> keys;
        for(auto const & entry : themap) {
            keys.push_back(entry.first);
        }
        return {TypeDescriptor::Strings, keys};
    }
}