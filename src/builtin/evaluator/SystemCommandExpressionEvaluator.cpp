/// (c) Ben Jones 2019

#include "SystemCommandExpressionEvaluator.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "statements/evaluator/StatementEvaluator.hpp"
#include "expressions/evaluator/IdentifierEvaluator.hpp"
#include "expressions/IdentifierExpression.hpp"
#include "parser/Parser.hpp"
#include "parser/LanguageException.hpp"
#include "statements/FunctionStatement.hpp"
#include "utility/ThreadManager.hpp"
#include <utility>

#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <array>

namespace {
    // from stack overflow
    std::string exec(const char* cmd) {
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }
}

namespace arrow {

    SystemCommandExpressionEvaluator::SystemCommandExpressionEvaluator(SystemCommandExpression expression)
      : m_expression(std::move(expression))
    {
    }
    Type SystemCommandExpressionEvaluator::evaluate(Environment & environment) const
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
            throw LanguageException("Exec function expects a string expression", callLineNumber);
        }
        try {
            auto const str = std::get<std::string>(t.m_variantType);
            auto const result = exec(str.c_str());
            return {TypeDescriptor::String, result};
        } catch (...) {
            throw LanguageException("Couldn't execute system command", callLineNumber);
        }
    }
}