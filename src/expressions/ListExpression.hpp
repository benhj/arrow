/// (c) Ben Jones 2019 - present

#pragma once

#include "Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>
#include <vector>

namespace arrow {
    
    class ListExpression final : public Expression
    {
      public:
        ListExpression(long const lineNumber);

        std::shared_ptr<ExpressionEvaluator> getEvaluator() const override;

        void addPart(std::shared_ptr<Expression> expression);
        int getPartsCount() const;

        std::vector<std::shared_ptr<Expression>> const & getParts() const;

        std::string toString() const override;

      private:
        std::vector<std::shared_ptr<Expression>> m_parts;
    };

}