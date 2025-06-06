/// (c) Ben Jones 2019 - present

#include "FolderListRecursiveFunctionExpressionEvaluator.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

#if defined(_HAS_CXX17)
#include <filesystem>
#endif
#include <stdexcept>
#include <string>

namespace arrow {

    FolderListRecursiveFunctionExpressionEvaluator::FolderListRecursiveFunctionExpressionEvaluator
    (FolderListRecursiveFunctionExpression expression)
      : m_expression(std::move(expression))
    {
    }
    Type FolderListRecursiveFunctionExpressionEvaluator::evaluate(Environment & environment) const
    {
        // Pull out the name of the function
        auto const callLineNumber = m_expression.getLineNumber();
        #if defined(_HAS_CXX17)
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
        try {
            auto const folderName = std::get<std::string>(t.m_variantType);
            std::vector<std::string> lines;

            using std::filesystem::recursive_directory_iterator;
            recursive_directory_iterator current(folderName);
            recursive_directory_iterator end;
            while(current != end) {
                auto const pathString = current->path().string();
                if(pathString == "." || pathString == "..") { 
                    continue;
                }
                lines.emplace_back(pathString);
                ++current;
            }
            return {TypeDescriptor::Strings, lines};
        } catch (...) {
            throw LanguageException("Problem with filesystem command", callLineNumber);
        }
        #else
        throw LanguageException("Not supported", callLineNumber);
        #endif
    }
}
