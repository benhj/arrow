#include "StartStatement.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include <utility>

namespace jasl {
    StartStatement::StartStatement() : Statement() {}
    StartStatement & StartStatement::withToken(Token token)
    {
        m_keywordToken = std::move(token);
        return *this;
    }
    void StartStatement::addBodyStatement(std::shared_ptr<Statement> bodyStatement)
    {
        m_bodyStatements.emplace_back(std::move(bodyStatement));
    }

    std::string StartStatement::toString() const
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

    std::shared_ptr<StatementEvaluator> StartStatement::getEvaluator() const
    {
        return nullptr;
    }
}