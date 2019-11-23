#pragma once

#include "Statement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>

namespace arrow {
    
    class SimpleArrowStatement : public Statement
    {
      public:
        SimpleArrowStatement();
        SimpleArrowStatement & withExpression(std::shared_ptr<Expression> expression);
        SimpleArrowStatement & withIdentifier(Token identifier);

        Token getIdentifier() const;
        std::shared_ptr<Expression> getExpression() const;

        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;

      private:
        std::shared_ptr<Expression> m_expression;
        Token m_identifier;
    };

}