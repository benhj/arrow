#pragma once

#include "Statement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>
#include <utility>

namespace jasl {
    class FunctionStatement : public Statement
    {
      public:
        FunctionStatement() : Statement() {}
        FunctionStatement & withToken(Token token)
        {
            m_keywordToken = std::move(token);
            return *this;
        }
        FunctionStatement & withTypeToken(Token token)
        {
            m_typeToken = std::move(token);
            return *this;
        }
        FunctionStatement & withNameToken(Token token)
        {
            m_nameToken = std::move(token);
            return *this;
        }
        FunctionStatement & withReturnIdentifierToken(Token token)
        {
            m_returnIdentifier = std::move(token);
            return *this;
        }
        FunctionStatement & 
        withExpressionCollection(std::shared_ptr<Expression> expression)
        {
            m_expressionCollection = std::move(expression);
            return *this;
        }
        void addBodyStatement(std::shared_ptr<Statement> bodyStatement)
        {
            m_bodyStatements.emplace_back(std::move(bodyStatement));
        }

        std::string toString() const override
        {
            std::string str("\nKeyword: ");
            str.append(m_keywordToken.raw);
            str.append("\nReturn type: ");
            str.append(m_typeToken.raw);
            str.append("\nFunction name: ");
            str.append(m_nameToken.raw);
            str.append(m_expressionCollection->toString());
            str.append("\nReturn identifier: ");
            str.append(m_returnIdentifier.raw);
            str.append("\nBegin body statements:\n");
            for(auto const & statement : m_bodyStatements) {
                str.append(statement->toString());
            }
            str.append("\nEnd body statements.");
            return str;
        }
        Token m_keywordToken; // fn keyword
        Token m_typeToken; // return type
        Token m_nameToken;
        std::shared_ptr<Expression> m_expressionCollection;
        Token m_returnIdentifier;
        std::vector<std::shared_ptr<Statement>> m_bodyStatements;
    };

}