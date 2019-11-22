#pragma once

#include "Statement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>

namespace jasl {
    
    class ArrowlessStatement : public Statement
    {
      public:
        ArrowlessStatement();
        ArrowlessStatement & withToken(Token token);
        ArrowlessStatement & withExpression(std::shared_ptr<Expression> expression);
        Token getToken() const;
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
      private:
        Token m_keywordToken; // the keyword (e.g. prn)
        std::shared_ptr<Expression> m_expression;
    };

}