/// (c) Ben Jones 2019 - present

#include "BuiltInFunctionExpressionParser.hpp"
#include "expressions/builtin/BuiltInFunctionExpressionBuilder.hpp"
#include "expressions/builtin/RandomFunctionExpression.hpp"
#include "expressions/builtin/SqrtFunctionExpression.hpp"
#include "expressions/builtin/StringInputExpression.hpp"
#include "expressions/builtin/SystemCommandExpression.hpp"
#include <utility>

namespace arrow {

    std::vector<std::shared_ptr<BuiltInFunctionExpressionBuilder>> 
    BuiltInFunctionExpressionParser::m_builtins;
    BuiltInFunctionExpressionParser::BuiltInFunctionExpressionParser(TokenManager & tm)
    : m_tm(tm)
    {
    }
    std::shared_ptr<Expression> BuiltInFunctionExpressionParser::parseExpression()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        if(m_builtins.empty()) {
            m_builtins.emplace_back(std::make_shared<RandomFunctionExpressionBuilder>());
        }
    }
}
