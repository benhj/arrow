/// (c) Ben Jones 2025 - present


#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include "parser/Parser.hpp"
#include "representation/PodType.hpp"
#include "GetKeyPressExpressionEvaluator.hpp"
#include <utility>

namespace arrow {

    GetKeyPressExpressionEvaluator::GetKeyPressExpressionEvaluator(GetKeyPressExpression expression)
      : m_expression(std::move(expression))
    {
    }
    Type GetKeyPressExpressionEvaluator::evaluate(Environment & environment) const
    {
        // Pull out the name of the function
        auto const callLineNumber = m_expression.getLineNumber();

        // Pull out the arguments being passed into the function
        auto const expression = m_expression.getExpression();
        auto const col = expression->getEvaluator()->evaluate(environment);
        auto & expressionCollEval = std::get<std::vector<Type>>(col.m_variantType);
        if(!expressionCollEval.empty()) {
            throw LanguageException("No arguments expected", callLineNumber);
        }

        auto optionalKeyPress = environment.retrieveKeyPress();
        PodType pod;
        if(optionalKeyPress) {
            pod.add("available", {TypeDescriptor::Bool, true});
            pod.add("key", {TypeDescriptor::Byte, *optionalKeyPress});
        } else {
            pod.add("available", {TypeDescriptor::Bool, false});
            pod.add("key", {TypeDescriptor::Byte, '\0'});
        }
        return {TypeDescriptor::Pod, pod};
        
    }
}