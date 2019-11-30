#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace arrow {
    
    class GroupedExpression : public Expression
    {
      public:
        GroupedExpression(long const lineNumber);

        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;

        DecayType decayType() const override;

        GroupedExpression & withExpression(std::shared_ptr<Expression> expression);
        
        std::shared_ptr<Expression> getExpression() const;
        std::string toString() const override;

      private:
        std::shared_ptr<Expression> m_expression;
    };

}