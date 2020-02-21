/// (c) Ben Jones 2020

#include "TokenizeFunctionExpressionEvaluator.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"

#include <sstream>
#include <utility>

namespace arrow {

    TokenizeFunctionExpressionEvaluator::TokenizeFunctionExpressionEvaluator(TokenizeFunctionExpression expression)
      : m_expression(std::move(expression))
    {
    }
    Type TokenizeFunctionExpressionEvaluator::evaluate(Environment & environment) const
    {
        auto const expression = m_expression.getExpression();
        auto const col = expression->getEvaluator()->evaluate(environment);
        auto const expressionCollEval = std::get<std::vector<Type>>(col.m_variantType);
        if(expressionCollEval.empty() || expressionCollEval.size() < 2) {
            throw LanguageException("Expected argument", m_expression.getLineNumber());
        }
        auto const stringEval = expressionCollEval[0];
        if(stringEval.m_descriptor != TypeDescriptor::String) {
            throw LanguageException("Bad type for tokenize", m_expression.getLineNumber());
        }
        auto const tokEval = expressionCollEval[1];
        if(tokEval.m_descriptor != TypeDescriptor::Byte) {
            throw LanguageException("Bad type for tokenize", m_expression.getLineNumber());
        }
        auto const text = std::get<std::string>(stringEval.m_variantType);
        auto const tok = std::get<char>(tokEval.m_variantType);

        std::istringstream ss(text);
        std::string token;
        std::vector<std::string> strs;
        while(std::getline(ss, token, tok)) {
            strs.push_back(token);
        }
        return {TypeDescriptor::Strings, strs};
    }
}