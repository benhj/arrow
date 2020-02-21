/// (c) Ben Jones 2020 - present


#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include "parser/Parser.hpp"
#include "TypeExpressionEvaluator.hpp"
#include <utility>

namespace arrow {

    TypeExpressionEvaluator::TypeExpressionEvaluator(TypeExpression expression)
      : m_expression(std::move(expression))
    {
    }
    Type TypeExpressionEvaluator::evaluate(Environment & cache) const
    {
        // Pull out the name of the function
        auto const callLineNumber = m_expression.getLineNumber();

        // Pull out the arguments being passed into the function
        auto const expression = m_expression.getExpression();

        std::string theType("nil");
        try {
            auto const col = expression->getEvaluator()->evaluate(cache);
            auto & expressionCollEval = std::get<std::vector<Type>>(col.m_variantType);
            if(expressionCollEval.empty()) {
                throw LanguageException("Expected argument", m_expression.getLineNumber());
            }
            auto & t = expressionCollEval[0];

            
            switch (t.m_descriptor) {
                case TypeDescriptor::List:
                    theType = "list";
                    break;
                case TypeDescriptor::Ints:
                    theType = "ints";
                    break;
                case TypeDescriptor::Reals:
                    theType = "reals";
                    break;
                case TypeDescriptor::Bools:
                    theType = "bools";
                    break;
                case TypeDescriptor::Strings:
                    theType = "strings";
                    break;
                case TypeDescriptor::Bytes:
                    theType = "bytes";
                    break;
                case TypeDescriptor::String:
                    theType = "string";
                    break;
                case TypeDescriptor::Byte:
                    theType = "byte";
                    break;
                case TypeDescriptor::Int:
                    theType = "int";
                    break;
                case TypeDescriptor::Real:
                    theType = "real";
                    break;
                case TypeDescriptor::Bool:
                    theType = "Bool";
                    break;
                case TypeDescriptor::Map:
                    theType = "map";
                    break;
                default:
                    break;
            }
        } catch (...) { /* not found, type is nil; the 'nothing' type */ }
        return {TypeDescriptor::String, theType};
    }
}