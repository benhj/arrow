/// (c) Ben Jones 2019 - present

#pragma once

#include "Expression.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace arrow {
    class QQStringExpression : public Expression
    {
      public:
        QQStringExpression(long const lineNumber);

        QQStringExpression & withQQStringToken(Token qQStringToken);

        Token const & getQQStringToken() const;

        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;

        std::string toString() const override;

      private:
        Token m_qQStringToken;
    };
}