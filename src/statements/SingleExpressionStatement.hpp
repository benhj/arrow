/// (c) Ben Jones 2019 - present

#pragma once

#include "Statement.hpp"
#include "expressions/Expression.hpp"
#include <memory>

namespace arrow {
    
    class SingleExpressionStatement : public Statement
    {
      public:
        SingleExpressionStatement(long const lineNumber);
        SingleExpressionStatement & withExpression(std::shared_ptr<Expression> expression);
        std::shared_ptr<Expression> getExpression() const;
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
      private:
        std::shared_ptr<Expression> m_expression;
    };

}