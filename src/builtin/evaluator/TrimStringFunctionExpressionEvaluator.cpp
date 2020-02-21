/// (c) Ben Jones 2019

#include "TrimStringFunctionExpressionEvaluator.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <math.h>
#include <algorithm>
#include <cctype>
#include <locale>
#include <utility>

namespace {

    // trim from start (in place)
    static inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
            return !std::isspace(ch) && ch != '\n';
        }));
    }

    // trim from end (in place)
    static inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
            return !std::isspace(ch) && ch != '\n';
        }).base(), s.end());
    }
}

namespace arrow {

    TrimStringFunctionExpressionEvaluator::TrimStringFunctionExpressionEvaluator(TrimStringFunctionExpression expression)
      : m_expression(std::move(expression))
    {
    }
    Type TrimStringFunctionExpressionEvaluator::evaluate(Environment & environment) const
    {
        auto const expression = m_expression.getExpression();
        auto const col = expression->getEvaluator()->evaluate(environment);
        auto const expressionCollEval = std::get<std::vector<Type>>(col.m_variantType);
        if(expressionCollEval.empty()) {
            throw LanguageException("Expected argument", m_expression.getLineNumber());
        }
        auto const eval = expressionCollEval[0];
        if(eval.m_descriptor != TypeDescriptor::String &&
           eval.m_descriptor != TypeDescriptor::ListWord) {
            throw LanguageException("String expected", m_expression.getLineNumber());

        }
        auto casted = std::get<std::string>(eval.m_variantType);
        ltrim(casted);
        rtrim(casted);
        return {eval.m_descriptor, casted};
    }
}