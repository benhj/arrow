#include "WhileStatement.hpp"
#include <utility>

namespace jasl {

    WhileStatement::WhileStatement() : Statement() {}
    WhileStatement & WhileStatement::withToken(Token token)
    {
        m_keywordToken = std::move(token);
        return *this;
    }
    WhileStatement & WhileStatement::withExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
        return *this;
    }
    void WhileStatement::addBodyStatement(std::shared_ptr<Statement> bodyStatement)
    {
        m_bodyStatements.emplace_back(std::move(bodyStatement));
    }

    std::string WhileStatement::toString() const
    {
        std::string str("\nKeyword: ");
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
}