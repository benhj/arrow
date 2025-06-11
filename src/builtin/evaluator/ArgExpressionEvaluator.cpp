/// (c) Ben Jones 2020 - present


#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include "parser/Parser.hpp"
#include "ArgExpressionEvaluator.hpp"
#include <utility>

namespace arrow {

    ArgExpressionEvaluator::ArgExpressionEvaluator(ArgExpression expression)
      : m_expression(std::move(expression))
    {
    }

    Type ArgExpressionEvaluator::evaluate(Environment & environment) const
    {
        // Pull out the name of the function
        auto const callLineNumber = m_expression.getLineNumber();

        // Pull out the arguments being passed into the function
        auto const expression = m_expression.getExpression();
        auto const col = expression->getEvaluator()->evaluate(environment);
        auto & expressionCollEval = std::get<std::vector<Type>>(col.m_variantType);
        if(expressionCollEval.empty()) {
            throw LanguageException("Expected argument", m_expression.getLineNumber());
        }
        auto & t = expressionCollEval[0];
        if(t.m_descriptor != TypeDescriptor::Int) {
            throw LanguageException("Expected an integer index", callLineNumber);
        }

        auto const index = std::get<int64_t>(t.m_variantType);
        try {
            return environment.getProgramArgument(index);
        } catch (...) {
            throw LanguageException("Error obtaining argument", callLineNumber);
        }
    }
}