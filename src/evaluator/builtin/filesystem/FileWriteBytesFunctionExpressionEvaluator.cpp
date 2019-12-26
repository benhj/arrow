/// (c) Ben Jones 2019

#include "FileWriteBytesFunctionExpressionEvaluator.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <string>

namespace arrow {

    FileWriteBytesFunctionExpressionEvaluator::FileWriteBytesFunctionExpressionEvaluator
    (FileWriteBytesFunctionExpression expression)
      : m_expression(std::move(expression))
    {
    }
    Type FileWriteBytesFunctionExpressionEvaluator::evaluate(Cache & cache) const
    {
        // Pull out the name of the function
        auto const callLineNumber = m_expression.getLineNumber();

        // Pull out the arguments being passed into the function
        auto const expression = m_expression.getExpression();
        auto const col = expression->getEvaluator()->evaluate(cache);
        auto const expressionCollEval = std::get<std::vector<Type>>(col.m_variantType);
        if(expressionCollEval.empty() || expressionCollEval.size() < 2) {
            throw LanguageException("Expected argument", m_expression.getLineNumber());
        }
        auto const t = expressionCollEval[0];
        if(t.m_descriptor != TypeDescriptor::Bytes) {
            throw LanguageException("Expects a bytes array", callLineNumber);
        }
        auto const s = expressionCollEval[1];
        if(s.m_descriptor != TypeDescriptor::String) {
            throw LanguageException("Expects a path", callLineNumber);
        }
        try {

            auto const bytes = std::get<std::vector<char>>(t.m_variantType);
            auto const filename = std::get<std::string>(s.m_variantType);
            std::ofstream file(filename, std::ios::binary);
            file.write(reinterpret_cast<char const*>(&bytes.front()), bytes.size());
            file.close();
            return {TypeDescriptor::Int, static_cast<int64_t>(bytes.size())};
        } catch (...) {
            throw LanguageException("Problem with filesystem command", callLineNumber);
        }
    }
}