#pragma once

#include "Statement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>

namespace arrow {
    
    class ArrowStatement : public Statement
    {
      public:
        ArrowStatement(long const lineNumber);
        ArrowStatement & withToken(Token token);
        ArrowStatement & withExpression(std::shared_ptr<Expression> expression);
        ArrowStatement & withIdentifier(Token identifier);

        Token getToken() const;
        std::shared_ptr<Expression> getExpression() const;
        Token getIdentifier() const;

        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;

      private:
        Token m_keywordToken; // the keyword (e.g. int)
        std::shared_ptr<Expression> m_expression;
        Token m_identifier;
    };

}