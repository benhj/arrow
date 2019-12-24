/// (c) Ben Jones 2019

#include "SystemCommandExpressionEvaluator.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include "evaluator/IdentifierEvaluator.hpp"
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
    Type SystemCommandExpressionEvaluator::evaluate(Cache & cache) const
    {
        // Pull out the name of the function
        auto const callLineNumber = m_expression.getLineNumber();

        // Pull out the arguments being passed into the function
        auto const expression = m_expression.getExpression();
        auto const t = expression->getEvaluator()->evaluate(cache);
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