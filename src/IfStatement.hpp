#pragma once

#include "ElseIfStatement.hpp"
#include "ElseStatement.hpp"
#include "Statement.hpp"
#include "Token.hpp"
#include "Expression.hpp"
#include <memory>
#include <utility>

namespace jasl {
    
    /// Note, an IfStatement can contain optional
    /// ElseIfStatement parts an an optional
    /// ElseStatement part
    class IfStatement : public Statement
    {
      public:
        IfStatement() : Statement() {}
        IfStatement & withToken(Token token)
        {
            m_keywordToken = std::move(token);
            return *this;
        }
        IfStatement & withExpression(std::shared_ptr<Expression> expression)
        {
            m_expression = std::move(expression);
            return *this;
        }
        void addBodyStatement(std::shared_ptr<Statement> bodyStatement)
        {
            m_bodyStatements.emplace_back(std::move(bodyStatement));
        }
        void addElseIfPart(std::shared_ptr<ElseIfStatement> elseIfPart)
        {
            m_elseIfParts.emplace_back(std::move(elseIfPart));
        }
        IfStatement & withElsePart(std::shared_ptr<ElseStatement> elsePart)
        {
            m_elsePart = std::move(elsePart);
            return *this;
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
            if(!m_elseIfParts.empty()) {
                str.append("\nBegin elseif parts:\n");
                for(auto const & elseifPart : m_elseIfParts) {
                    str.append(elseifPart->toString());
                }
                str.append("\nEnd elseif part.\n");
            }
            if(m_elsePart) {
                str.append("\nBegin else part:\n");
                str.append(m_elsePart->toString());
                str.append("\nEnd else part.");
            }
            return str;
        }
        Token m_keywordToken; // the if keyword
        std::shared_ptr<Expression> m_expression;
        std::vector<std::shared_ptr<Statement>> m_bodyStatements;
        std::vector<std::shared_ptr<ElseIfStatement>> m_elseIfParts;
        std::shared_ptr<ElseStatement> m_elsePart;
    };

}