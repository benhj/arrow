/// (c) Ben Jones 2020 - present

#include "WildcardEqExpressionEvaluator.hpp"
#include "WildcardMatch.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"

#include <regex>
#include <utility>

namespace arrow {

    WildcardEqExpressionEvaluator::WildcardEqExpressionEvaluator(WildcardEqExpression expression)
      : m_expression(std::move(expression))
    {
    }
    Type WildcardEqExpressionEvaluator::evaluate(Environment & environment) const
    {
        auto const expression = m_expression.getExpression();
        auto const col = expression->getEvaluator()->evaluate(environment);
        auto const expressionCollEval = std::get<std::vector<Type>>(col.m_variantType);
        if(expressionCollEval.empty() || expressionCollEval.size() < 2) {
            throw LanguageException("Expected argument", m_expression.getLineNumber());
        }
        auto const stringEval = expressionCollEval[0];
        if(stringEval.m_descriptor != TypeDescriptor::String) {
            throw LanguageException("Bad type for wildcard_eq", m_expression.getLineNumber());
        }
        auto const regexEval = expressionCollEval[1];
        if(regexEval.m_descriptor != TypeDescriptor::String) {
            throw LanguageException("Bad type for wildcard_eq", m_expression.getLineNumber());
        }
        auto const text = std::get<std::string>(stringEval.m_variantType);
        auto const pattern = std::get<std::string>(regexEval.m_variantType);
        auto const result = wildcardMatch(pattern.c_str(), text.c_str());
        return {TypeDescriptor::Bool, result};
    }
}