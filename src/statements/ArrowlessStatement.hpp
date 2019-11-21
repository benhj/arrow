#pragma once

#include "Statement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>
#include <utility>

namespace jasl {
    
    class ArrowlessStatement : public Statement
    {
      public:
        ArrowlessStatement() : Statement() {}
        ArrowlessStatement & withToken(Token token)
        {
            m_keywordToken = std::move(token);
            return *this;
        }
        ArrowlessStatement & withExpression(std::shared_ptr<Expression> expression)
        {
            m_expression = std::move(expression);
            return *this;
        }
        Token getToken() const
        {
            return m_keywordToken;
        }

        std::string toString() const override
        {
            std::string str("\nKeyword: ");
            str.append(m_keywordToken.raw);
            str.append("\nExpression: ");
            str.append(m_expression->toString());
            return str;
        }

        Token m_keywordToken; // the keyword (e.g. prn)
        std::shared_ptr<Expression> m_expression;
    };

}