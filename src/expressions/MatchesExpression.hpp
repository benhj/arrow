/// (c) Ben Jones 2019 - present

#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>
#include <vector>

namespace arrow {
    
    class MatchesExpression final : public Expression
    {
      public:
        MatchesExpression(long const lineNumber);

        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;
        MatchesExpression & withLeftExpression(std::shared_ptr<Expression> expression);
        MatchesExpression & withRightExpression(std::shared_ptr<Expression> expression);
        std::shared_ptr<Expression> const & getLeftExpression() const;
        std::shared_ptr<Expression> const & getRightExpression() const;
        std::string toString() const override;

      private:
        std::shared_ptr<Expression> m_left;
        std::shared_ptr<Expression> m_right;
    };

}