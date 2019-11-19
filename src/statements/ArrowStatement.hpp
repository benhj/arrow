#pragma once

#include "Statement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>
#include <utility>

namespace jasl {
    
    class ArrowStatement : public Statement
    {
      public:
        ArrowStatement() : Statement() {}
        ArrowStatement & withToken(Token token)
        {
            m_keywordToken = std::move(token);
            return *this;
        }
        ArrowStatement & withExpression(std::shared_ptr<Expression> expression)
        {
            m_expression = std::move(expression);
            return *this;
        }
        ArrowStatement & withIdentifier(Token identifier)
        {
            m_identifier = std::move(identifier);
            return *this;
        }

        std::string toString() const override
        {
            std::string str("\nKeyword: ");
            str.append(m_keywordToken.raw);
            str.append("\nExpression: ");
            str.append(m_expression->toString());
            str.append("\nIdentifier: ");
            str.append(m_identifier.raw);
            return str;
        }

        Token m_keywordToken; // the keyword (e.g. int)
        std::shared_ptr<Expression> m_expression;
        Token m_identifier;
    };

}