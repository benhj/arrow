#pragma once

#include "Statement.hpp"
#include "expressions/Expression.hpp"
#include "expressions/ExpressionCollectionExpression.hpp"
#include "lexer/Token.hpp"
#include <memory>
#include <utility>

namespace jasl {

    class CallStatement : public Statement
    {
      public:
        CallStatement();
        CallStatement & withToken(Token token);
        CallStatement & withFunctionNameToken(Token token);
        CallStatement & 
        withExpressionCollection(std::shared_ptr<Expression> expression);
        CallStatement & withIdentifier(Token identifier);

        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;

      private:
        Token m_keywordToken; // the call keyword
        Token m_functionNameToken; // the call keyword
        std::shared_ptr<Expression> m_expressionCollection;
        Token m_identifier; // optional
    };

}