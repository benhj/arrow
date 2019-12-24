/// (c) Ben Jones 2019 - present

#pragma once

#include "TokenManager.hpp"
#include "expressions/Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>
#include <vector>

namespace arrow {

    class BuiltInFunctionExpressionBuilder;
    class ExpressionParser;
    class BuiltInFunctionExpressionParser
    {
      public:
        BuiltInFunctionExpressionParser(TokenManager & tm);
        std::shared_ptr<Expression> parseExpression();

      private:
        TokenManager & m_tm;
        static std::vector<std::shared_ptr<BuiltInFunctionExpressionBuilder>> m_builtins;
    };
}
