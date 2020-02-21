/// (c) Ben Jones 2019

#include "statements/evaluator/StatementEvaluator.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "expressions/evaluator/IdentifierEvaluator.hpp"
#include "expressions/IdentifierExpression.hpp"
#include "parser/LanguageException.hpp"
#include "parser/Parser.hpp"
#include "RandomFunctionExpressionEvaluator.hpp"
#include "statements/FunctionStatement.hpp"
#include "utility/ThreadManager.hpp"
#include <utility>
#include <random>

namespace arrow {

    namespace {

        std::random_device rd;
        std::default_random_engine re(rd());

        long getRandomLong(long const upper)
        {
            using Dist = std::uniform_int_distribution<long>;
            static Dist uid {};
            return uid(re, Dist::param_type{0, upper});
        }
    }


    RandomFunctionExpressionEvaluator::RandomFunctionExpressionEvaluator(RandomFunctionExpression expression)
      : m_expression(std::move(expression))
    {
    }
    Type RandomFunctionExpressionEvaluator::evaluate(Environment & environment) const
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

        if(t.m_descriptor == TypeDescriptor::Int) {
            auto const val = std::get<int64_t>(t.m_variantType);
            using Dist = std::uniform_int_distribution<int64_t>;
            static Dist uid {};
            return {TypeDescriptor::Int, uid(re, Dist::param_type{0, val})};
        } else if(t.m_descriptor == TypeDescriptor::Real) {
            using Dist = std::uniform_real_distribution<real>;
            static Dist udd {};
            auto const val = std::get<real>(t.m_variantType);
            return {TypeDescriptor::Real, udd(re, Dist::param_type{0, val})};
        } else if(t.m_descriptor == TypeDescriptor::List) {
            auto const list = std::get<std::vector<Type>>(t.m_variantType);
            return list[getRandomLong(list.size() - 1)];
        } else if(t.m_descriptor == TypeDescriptor::String) {
            auto const str = std::get<std::string>(t.m_variantType);
            return {TypeDescriptor::Bytes, str[getRandomLong(str.size() - 1)]};
        } else if(t.m_descriptor == TypeDescriptor::Strings) {
            auto const vec = std::get<std::vector<std::string>>(t.m_variantType);
            return {TypeDescriptor::String, vec[getRandomLong(vec.size() - 1)]};
        } else if(t.m_descriptor == TypeDescriptor::Bytes) {
            auto const vec = std::get<std::vector<char>>(t.m_variantType);
            return {TypeDescriptor::Byte, vec[getRandomLong(vec.size() - 1)]};
        } else if(t.m_descriptor == TypeDescriptor::Ints) {
            auto const vec = std::get<std::vector<int64_t>>(t.m_variantType);
            return {TypeDescriptor::Int, vec[getRandomLong(vec.size() - 1)]};
        } else if(t.m_descriptor == TypeDescriptor::Reals) {
            auto const vec = std::get<std::vector<real>>(t.m_variantType);
            return {TypeDescriptor::Real, vec[getRandomLong(vec.size() - 1)]};
        }   

        else {
            throw LanguageException("Bad type for random", callLineNumber);
        }

        return t;
    }
}