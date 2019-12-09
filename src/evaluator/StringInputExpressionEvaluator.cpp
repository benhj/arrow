/// (c) Ben Jones 2019

#include "StringInputExpressionEvaluator.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include "evaluator/IdentifierEvaluator.hpp"
#include "expressions/IdentifierExpression.hpp"
#include "parser/Parser.hpp"
#include "parser/LanguageException.hpp"
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


    StringInputExpressionEvaluator::StringInputExpressionEvaluator(StringInputExpression expression)
      : m_expression(std::move(expression))
    {
    }
    Type StringInputExpressionEvaluator::evaluate(Cache & cache) const
    {
        // Pull out the name of the function
        auto const callLineNumber = m_expression.getLineNumber();

        // Pull out the arguments being passed into the function
        auto const expression = m_expression.getExpression();
        auto const t = expression->getEvaluator()->evaluate(cache);
        std::cout<<t.toString();
        std::string result;
        std::getline(std::cin, result);
        return {TypeDescriptor::String, result};
    }
}