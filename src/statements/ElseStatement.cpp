#include "ElseStatement.hpp"
#include <utility>

namespace jasl {

    ElseStatement::ElseStatement() : Statement() {}
    ElseStatement & ElseStatement::withToken(Token token)
    {
        m_keywordToken = std::move(token);
        return *this;
    }
    void ElseStatement::addBodyStatement(std::shared_ptr<Statement> bodyStatement)
    {
        m_bodyStatements.emplace_back(std::move(bodyStatement));
    }

    std::string ElseStatement::toString() const
    {
        std::string str("\nKeyword: ");
        str.append(m_keywordToken.raw);
        str.append("\nBegin body statements:\n");
        for(auto const & statement : m_bodyStatements) {
            str.append(statement->toString());
        }
        str.append("\nEnd body statements.");
        return str;
    }
}