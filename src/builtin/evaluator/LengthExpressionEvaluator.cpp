/// (c) Ben Jones 2020 - present

#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include "parser/Parser.hpp"
#include "LengthExpressionEvaluator.hpp"
#include <utility>

namespace arrow {

    namespace {
        template <typename T>
        size_t getLength(Type & t) {
            auto & casted = std::get<std::vector<T>>(t.m_variantType);
            return casted.size();
        }
    }

    LengthExpressionEvaluator::LengthExpressionEvaluator(LengthExpression expression)
      : m_expression(std::move(expression))
    {
    }
    Type LengthExpressionEvaluator::evaluate(Environment & cache) const
    {
        // Pull out the name of the function
        auto const callLineNumber = m_expression.getLineNumber();

        // Pull out the arguments being passed into the function
        auto const expression = m_expression.getExpression();
        auto const col = expression->getEvaluator()->evaluate(cache);
        auto & expressionCollEval = std::get<std::vector<Type>>(col.m_variantType);
        if(expressionCollEval.empty()) {
            throw LanguageException("Expected argument", m_expression.getLineNumber());
        }
        auto & t = expressionCollEval[0];

        if(t.m_descriptor != TypeDescriptor::List &&
           t.m_descriptor != TypeDescriptor::Ints &&
           t.m_descriptor != TypeDescriptor::Reals &&
           t.m_descriptor != TypeDescriptor::Strings &&
           t.m_descriptor != TypeDescriptor::Bools &&
           t.m_descriptor != TypeDescriptor::Bytes &&
           t.m_descriptor != TypeDescriptor::String) {
            throw LanguageException("Not a container", m_expression.getLineNumber());
        }

        size_t theLength = -1;
        switch (t.m_descriptor) {
            case TypeDescriptor::List:
                theLength = getLength<Type>(t);
                break;
            case TypeDescriptor::Ints:
                theLength = getLength<int64_t>(t);
                break;
            case TypeDescriptor::Reals:
                theLength = getLength<real>(t);
                break;
            case TypeDescriptor::Bools:
                theLength = getLength<bool>(t);
                break;
            case TypeDescriptor::Strings:
                theLength = getLength<std::string>(t);
                break;
            case TypeDescriptor::Bytes:
                theLength = getLength<char>(t);
                break;
            case TypeDescriptor::String: {
                auto & casted = std::get<std::string>(t.m_variantType);
                theLength = casted.size();
            }
            default:
                break;
        }
        return {TypeDescriptor::Int, static_cast<int64_t>(theLength)};
    }
}