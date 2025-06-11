/// (c) Ben Jones 2025 - present


#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include "parser/Parser.hpp"
#include "InitKeyHandlerFunctionExpressionEvaluator.hpp"
#include <utility>

namespace arrow {

    InitKeyHandlerFunctionExpressionEvaluator::InitKeyHandlerFunctionExpressionEvaluator(InitKeyHandlerFunctionExpression expression)
      : m_expression(std::move(expression))
    {
    }
    Type InitKeyHandlerFunctionExpressionEvaluator::evaluate(Environment & environment) const
    {
        // Pull out the name of the function
        auto const callLineNumber = m_expression.getLineNumber();

        // Pull out the arguments being passed into the function
        auto const expression = m_expression.getExpression();
        auto const col = expression->getEvaluator()->evaluate(environment);
        auto & expressionCollEval = std::get<std::vector<Type>>(col.m_variantType);
        if(!expressionCollEval.empty()) {
            throw LanguageException("No args expected", callLineNumber);
        }  

        // Proceed
        environment.initializeKeyHandler();
        return {TypeDescriptor::Bool, true};
    }
}