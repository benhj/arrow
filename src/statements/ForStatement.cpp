#include "ForStatement.hpp"
#include <utility>

namespace jasl {

    ForStatement::ForStatement() : Statement() {}
    ForStatement & ForStatement::withToken(Token token)
    {
        m_keywordToken = std::move(token);
        return *this;
    }
    ForStatement & ForStatement::withIdentifierA(Token identifier)
    {
        m_identifierA = std::move(identifier);
        return *this;
    }
    ForStatement & ForStatement::withIdentifierB(Token identifier)
    {
        m_identifierB = std::move(identifier);
        return *this;
    }
    void ForStatement::addBodyStatement(std::shared_ptr<Statement> bodyStatement)
    {
        m_bodyStatements.emplace_back(std::move(bodyStatement));
    }

    std::string ForStatement::toString() const
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
}