/// (c) Ben Jones 2019

#include "RandomFunctionExpressionEvaluator.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include "evaluator/IdentifierEvaluator.hpp"
#include "expressions/IdentifierExpression.hpp"
#include "parser/Parser.hpp"
#include "parser/LanguageException.hpp"
#include "statements/FunctionStatement.hpp"
#include "utility/ThreadManager.hpp"
#include <utility>

namespace arrow {
    RandomFunctionExpressionEvaluator::RandomFunctionExpressionEvaluator(RandomFunctionExpression expression)
      : m_expression(std::move(expression))
    {
    }
    Type RandomFunctionExpressionEvaluator::evaluate(Cache & cache) const
    {
        // Pull out the name of the function
        auto const callLineNumber = m_expression.getLineNumber();

        // Pull out the arguments being passed into the function
        auto const expression = m_expression.getExpression();
        auto const t = expression->getEvaluator()->evaluate(cache);

        static std::default_random_engine re(m_r());

        if(t.m_descriptor == TypeDescriptor::Int) {
            auto const val = std::get<int64_t>(t.m_variantType);
            using Dist = std::uniform_int_distribution<int64_t>;
            static Dist uid {};
            return {TypeDescriptor::Int, uid(re, Dist::param_type{0, val})};
        } else if(t.m_descriptor == TypeDescriptor::Real) {
            using Dist = std::uniform_real_distribution<long double>;
            static Dist udd {};
            auto const val = std::get<long double>(t.m_variantType);
            return {TypeDescriptor::Real, udd(re, Dist::param_type{0, val})};
        } else if(t.m_descriptor == TypeDescriptor::List) {
            using Dist = std::uniform_int_distribution<long>;
            static Dist uid {};
            auto const list = std::get<std::vector<Type>>(t.m_variantType);
            auto const length = static_cast<long>(list.size());
            auto random = uid(re, Dist::param_type{0, length - 1});
            return list[random];
        } else if(t.m_descriptor == TypeDescriptor::String) {
            using Dist = std::uniform_int_distribution<long>;
            static Dist uid {};
            auto const str = std::get<std::string>(t.m_variantType);
            auto const length = static_cast<long>(str.size());
            auto random = uid(re, Dist::param_type{0, length - 1});
            return {TypeDescriptor::Bytes, str[random]};
        }

        else {
            throw LanguageException("Bad type for random", callLineNumber);
        }

        return t;
    }
}