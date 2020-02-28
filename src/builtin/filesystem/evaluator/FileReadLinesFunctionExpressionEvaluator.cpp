/// (c) Ben Jones 2019 - present

#include "FileReadLinesFunctionExpressionEvaluator.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

#include <fstream>
#include <stdexcept>
#include <sstream>
#include <string>

namespace arrow {

    FileReadLinesFunctionExpressionEvaluator::FileReadLinesFunctionExpressionEvaluator
    (FileReadLinesFunctionExpression expression)
      : m_expression(std::move(expression))
    {
    }
    Type FileReadLinesFunctionExpressionEvaluator::evaluate(Environment & environment) const
    {
        // Pull out the name of the function
        auto const callLineNumber = m_expression.getLineNumber();

        // Pull out the arguments being passed into the function
        auto const expression = m_expression.getExpression();
        auto const col = expression->getEvaluator()->evaluate(environment);
        auto const expressionCollEval = std::get<std::vector<Type>>(col.m_variantType);
        if(expressionCollEval.empty()) {
            throw LanguageException("Expected argument", m_expression.getLineNumber());
        }
        auto const t = expressionCollEval[0];
        if(t.m_descriptor != TypeDescriptor::String) {
            throw LanguageException("Expects a string expression", callLineNumber);
        }
        char tok = '\n';
        if(expressionCollEval.size() > 1) {
            auto const thechar = expressionCollEval[1];
            if(thechar.m_descriptor != TypeDescriptor::Byte) {
                throw LanguageException("Expects a char token", callLineNumber);
            }
            tok = std::get<char>(thechar.m_variantType);
        }

        try {
            auto const filename = std::get<std::string>(t.m_variantType);
            std::vector<std::string> lines;

            // read the data:
            std::ifstream input(filename);
            if(!input.is_open()) {
                throw LanguageException("Problem opening file for reading", callLineNumber);
            }
            for( std::string line; getline( input, line); ) {
                if(tok != '\n') {
                    std::stringstream temp;
                    temp << line;
                    for( std::string line2; getline( temp, line2, tok); ) {
                        lines.push_back(line2);
                    }
                } else {
                    // split purely on '\n'
                    lines.push_back(line);
                }
            }
            return {TypeDescriptor::Strings, lines};
        } catch (...) {
            throw LanguageException("Problem with filesystem command", callLineNumber);
        }
    }
}