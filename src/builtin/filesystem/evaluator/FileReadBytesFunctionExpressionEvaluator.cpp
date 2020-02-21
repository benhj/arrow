/// (c) Ben Jones 2019

#include "FileReadBytesFunctionExpressionEvaluator.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <string>

namespace arrow {

    FileReadBytesFunctionExpressionEvaluator::FileReadBytesFunctionExpressionEvaluator
    (FileReadBytesFunctionExpression expression)
      : m_expression(std::move(expression))
    {
    }
    Type FileReadBytesFunctionExpressionEvaluator::evaluate(Environment & cache) const
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
            throw LanguageException("Expects a string expression", callLineNumber);
        }
        try {
            auto const filename = std::get<std::string>(t.m_variantType);

            // open the file:
            std::ifstream file(filename, std::ios::binary);

            // Stop eating new lines in binary mode!!!
            file.unsetf(std::ios::skipws);

            // Get its size
            file.seekg(0, std::ios::end);
            auto const fileSize = file.tellg();
            file.seekg(0, std::ios::beg);

            // reserve capacity
            std::vector<char> ba;
            ba.reserve(fileSize);

            // read the data:
            ba.insert(std::begin(ba),
                      std::istream_iterator<char>(file),
                      std::istream_iterator<char>());

            return {TypeDescriptor::Bytes, ba};
        } catch (...) {
            throw LanguageException("Problem with filesystem command", callLineNumber);
        }
    }
}