/// (c) Ben Jones 2025 - present

#pragma once

#include "builtin/BuiltInFunctionExpression.hpp"
#include "builtin/BuiltInFunctionExpressionBuilder.hpp"
#include "expressions/Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>
#include <vector>

namespace arrow {

    class FillCircleFunctionExpression : public Expression, public BuiltInFunctionExpression
    {
      public:
        FillCircleFunctionExpression(long const lineNumber);
        void setExpression(std::shared_ptr<Expression> expression) override;
        std::shared_ptr<Expression> const & getExpression() const;
        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;
        std::string toString() const override;
      private:
        std::shared_ptr<Expression> m_expression;
    };

    class FillCircleFunctionExpressionBuilder : public BuiltInFunctionExpressionBuilder
    {
      public:
        std::string getName() const override {
            return "fillCircle"; // Changed to fillCircle
        }
        std::shared_ptr<Expression> build(long const ln) const override {
            return std::make_shared<FillCircleFunctionExpression>(ln);
        }
    };

} // namespace arrow