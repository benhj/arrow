#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>
#include <vector>

namespace jasl {
    
    class ListExpression : public Expression
    {
      public:
        ListExpression();

        std::shared_ptr<Evaluator> getEvaluator() const override;

        Type evaluate() const override;

        DecayType decayType() const override;

        void addPart(std::shared_ptr<Expression> expression);

        int getPartsCount() const;
        
        std::string toString() const override;

      private:
        std::vector<std::shared_ptr<Expression>> m_parts;
    };

}