/// (c) Ben Jones 2020 - present

#pragma once

#include "BuiltInFunctionExpression.hpp"
#include "BuiltInFunctionExpressionBuilder.hpp"
#include "expressions/Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>
#include <vector>

namespace arrow {
    
    class MapContainsFunctionExpression final : public Expression, public BuiltInFunctionExpression
    {
      public:
        MapContainsFunctionExpression(long const lineNumber);
        void setExpression(std::shared_ptr<Expression> expression) override;
        std::shared_ptr<Expression> const & getExpression() const;
        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;
        std::string toString() const override;
      private:
        std::shared_ptr<Expression> m_expression;
    };

    class MapContainsFunctionExpressionBuilder final : public BuiltInFunctionExpressionBuilder
    {
      public:
        std::string getName() const override {
            return "map_contains";
        }
        std::shared_ptr<Expression> build(long const ln) const override {
            return std::make_shared<MapContainsFunctionExpression>(ln);
        }
    };

}