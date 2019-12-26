/// (c) Ben Jones 2019

#include "FileReadLinesFunctionExpressionEvaluator.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

#include <fstream>
#include <stdexcept>
#include <string>

namespace arrow {

    FileReadLinesFunctionExpressionEvaluator::FileReadLinesFunctionExpressionEvaluator(FileReadLinesFunctionExpression expression)
      : m_expression(std::move(expression))
    {
    }
    Type FileReadLinesFunctionExpressionEvaluator::evaluate(Cache & cache) const
    {
        // Pull out the name of the function
        auto const callLineNumber = m_expression.getLineNumber();

        // Pull out the arguments being passed into the function
        auto const expression = m_expression.getExpression();
        auto const col = expression->getEvaluator()->evaluate(cache);
        auto const expressionCollEval = std::get<std::vector<Type>>(col.m_variantType);
        if(expressionCollEval.empty()) {
            throw LanguageException("Expected argument", m_expression.getLineNumber());
        }
        auto const t = expressionCollEval[0];
        if(t.m_descriptor != TypeDescriptor::String) {
            throw LanguageException("Exec function expects a string expression", callLineNumber);
        }
        try {
            auto const filename = std::get<std::string>(t.m_variantType);
            std::vector<std::string> lines;

            // read the data:
            std::ifstream input(filename);
            for( std::string line; getline( input, line ); ) {
                lines.push_back(line);
            }
            return {TypeDescriptor::Strings, lines};
        } catch (...) {
            throw LanguageException("Problem with filesystem command", callLineNumber);
        }
    }
}