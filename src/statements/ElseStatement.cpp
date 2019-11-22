#include "ElseStatement.hpp"
#include "evaluator/StatementEvaluator.hpp"
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
    std::vector<std::shared_ptr<Statement>> 
    ElseStatement::getBodyStatements() const
    {
        return m_bodyStatements;
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

    std::shared_ptr<StatementEvaluator> ElseStatement::getEvaluator() const
    {
        return nullptr;
    }
}