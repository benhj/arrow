#pragma once

#include "Expression.hpp"
#include "ExpressionCollectionExpression.hpp"
#include "Statement.hpp"
#include "Token.hpp"
#include <memory>
#include <utility>

namespace jasl {

    class CallStatement : public Statement
    {
      public:
        CallStatement() : Statement() {}
        CallStatement & withToken(Token token)
        {
            m_keywordToken = std::move(token);
            return *this;
        }
        CallStatement & withFunctionNameToken(Token token)
        {
            m_functionNameToken = std::move(token);
            return *this;
        }
        CallStatement & 
        withExpressionCollection(std::shared_ptr<Expression> expression)
        {
            m_expressionCollection = std::move(expression);
            return *this;
        }
        CallStatement & withIdentifier(Token identifier)
        {
            m_identifier = std::move(identifier);
            return *this;
        }

        std::string toString() const override
        {
            std::string str("\nKeyword: ");
            str.append(m_keywordToken.raw);
            str.append("\nFunction name: ");
            str.append(m_functionNameToken.raw);
            str.append(m_expressionCollection->toString());
            str.append("\nIdentifier: ");
            str.append(m_identifier.raw);
            return str;
        }

        Token m_keywordToken; // the call keyword
        Token m_functionNameToken; // the call keyword
        std::shared_ptr<Expression> m_expressionCollection;
        Token m_identifier; // optional
    };

}