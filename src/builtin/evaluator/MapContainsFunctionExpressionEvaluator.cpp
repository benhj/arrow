/// (c) Ben Jones 2020 - present

#include "MapContainsFunctionExpressionEvaluator.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <math.h>
#include <utility>

namespace arrow {

    MapContainsFunctionExpressionEvaluator::MapContainsFunctionExpressionEvaluator(MapContainsFunctionExpression expression)
      : m_expression(std::move(expression))
    {
    }
    Type MapContainsFunctionExpressionEvaluator::evaluate(Environment & cache) const
    {
        auto const expression = m_expression.getExpression();
        auto const col = expression->getEvaluator()->evaluate(cache);
        auto & expressionCollEval = std::get<std::vector<Type>>(col.m_variantType);
        if(expressionCollEval.empty()) {
            throw LanguageException("Expected arguments", m_expression.getLineNumber());
        }
        if(expressionCollEval.size() < 2) {
            throw LanguageException("Expected extra argument", m_expression.getLineNumber());
        }
        auto & mapEval = expressionCollEval[0];
        auto & keyEval = expressionCollEval[1];
        if(mapEval.m_descriptor != TypeDescriptor::Map) {
            throw LanguageException("Not a map", m_expression.getLineNumber());
        }
        if(keyEval.m_descriptor != TypeDescriptor::String &&
           keyEval.m_descriptor != TypeDescriptor::ListWord) {
            throw LanguageException("Bad key expression", m_expression.getLineNumber());
        }
        auto & themap = std::get<std::map<std::string, Type>>(mapEval.m_variantType);
        auto & thekey = std::get<std::string>(keyEval.m_variantType);
        auto found = themap.find(thekey);
        auto result = (found != std::end(themap));
        return {TypeDescriptor::Bool, result};
    }
}