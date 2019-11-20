#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace jasl {
    
    class GroupedExpression : public Expression
    {
      public:
        GroupedExpression();

        std::shared_ptr<Evaluator> getEvaluator() const override;

        DecayType decayType() const override;

        GroupedExpression & withExpression(std::shared_ptr<Expression> expression);
        
        std::shared_ptr<Expression> getExpression() const;
        std::string toString() const override;

      private:
        std::shared_ptr<Expression> m_expression;
    };

}