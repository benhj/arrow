/// (c) Ben Jones 2019

#include "RegexFunctionExpressionEvaluator.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"

#include <regex>
#include <utility>

namespace {
    inline
    std::vector<std::string> extract(std::string const & text,
                                     std::string const & reg)
    {
        static std::regex const hl_regex(reg, std::regex_constants::icase) ;
        return { std::sregex_token_iterator( text.begin(), text.end(), hl_regex, 1 ),
                 std::sregex_token_iterator{} } ;
    }
}


namespace arrow {

    RegexFunctionExpressionEvaluator::RegexFunctionExpressionEvaluator(RegexFunctionExpression expression)
      : m_expression(std::move(expression))
    {
    }
    Type RegexFunctionExpressionEvaluator::evaluate(Environment & environment) const
    {
        auto const expression = m_expression.getExpression();
        auto const col = expression->getEvaluator()->evaluate(environment);
        auto const expressionCollEval = std::get<std::vector<Type>>(col.m_variantType);
        if(expressionCollEval.empty() || expressionCollEval.size() < 2) {
            throw LanguageException("Expected argument", m_expression.getLineNumber());
        }
        auto const stringEval = expressionCollEval[0];
        if(stringEval.m_descriptor != TypeDescriptor::String) {
            throw LanguageException("Bad type for regex", m_expression.getLineNumber());
        }
        auto const regexEval = expressionCollEval[1];
        if(regexEval.m_descriptor != TypeDescriptor::String) {
            throw LanguageException("Bad type for regex", m_expression.getLineNumber());
        }
        auto const text = std::get<std::string>(stringEval.m_variantType);
        auto const reg = std::get<std::string>(regexEval.m_variantType);
        auto const result = extract(text, reg);
        return {TypeDescriptor::Strings, result};
    }
}