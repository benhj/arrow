#pragma once

#include "Statement.hpp"
#include "Token.hpp"
#include "Expression.hpp"
#include <memory>
#include <utility>

namespace jasl {
    
    class ForStatement : public Statement
    {
      public:
        ForStatement() : Statement() {}
        ForStatement & withToken(Token token)
        {
            m_keywordToken = std::move(token);
            return *this;
        }
        ForStatement & withIdentifierA(Token identifier)
        {
            m_identifierA = std::move(identifier);
            return *this;
        }
        ForStatement & withIdentifierB(Token identifier)
        {
            m_identifierB = std::move(identifier);
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
            str.append("\nIdentifier A: ");
            str.append(m_identifierA.raw);
            str.append("\nIdentifier B: ");
            str.append(m_identifierB.raw);
            str.append("\nBegin body statements:\n");
            for(auto const & statement : m_bodyStatements) {
                str.append(statement->toString());
            }
            str.append("\nEnd body statements.");
            return str;
        }

        Token m_keywordToken; // the for keyword
        Token m_identifierA;
        Token m_identifierB;
        std::vector<std::shared_ptr<Statement>> m_bodyStatements;
    };

}