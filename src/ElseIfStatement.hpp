#pragma once

#include "Statement.hpp"
#include "Token.hpp"
#include "Expression.hpp"
#include <memory>
#include <utility>

namespace jasl {
    
    /// Note, an ElseIfStatement can only be part
    /// of an IfStatement
    class ElseIfStatement : public Statement
    {
      public:
        ElseIfStatement() : Statement() {}
        ElseIfStatement & withToken(Token token)
        {
            m_keywordToken = std::move(token);
            return *this;
        }
        ElseIfStatement & withExpression(std::shared_ptr<Expression> expression)
        {
            m_expression = std::move(expression);
            return *this;
        }
        void addBodyStatement(std::shared_ptr<Statement> bodyStatement)
        {
            m_bodyStatements.emplace_back(std::move(bodyStatement));
        }

        std::string toString() const override
        {
            std::string str("Keyword: ");
            str.append(m_keywordToken.raw);
            str.append("\nExpression: ");
            str.append(m_expression->toString());
            str.append("\nBegin body statements:\n");
            for(auto const & statement : m_bodyStatements) {
                str.append(statement->toString());
            }
            str.append("\nEnd body statements.");
            return str;
        }
        Token m_keywordToken; // the elseif keyword
        std::shared_ptr<Expression> m_expression;
        std::vector<std::shared_ptr<Statement>> m_bodyStatements;
    };

}