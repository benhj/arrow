/// (c) Ben Jones 2019 - present

#include "ForStatement.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include "evaluator/ForStatementEvaluator.hpp"
#include <utility>

namespace arrow {

    ForStatement::ForStatement(long const lineNumber) 
      : Statement(lineNumber)
    {
    }
    ForStatement & ForStatement::withToken(Token token)
    {
        m_keywordToken = std::move(token);
        return *this;
    }
    ForStatement & ForStatement::withIndexer(Token token)
    {
        m_indexer = std::move(token);
        return *this;
    }
    ForStatement & ForStatement::withIdentifier(Token token)
    {
        m_identifier = std::move(token);
        return *this;
    }
    void ForStatement::addBodyStatement(std::shared_ptr<Statement> bodyStatement)
    {
        m_bodyStatements.emplace_back(std::move(bodyStatement));
    }
    std::vector<std::shared_ptr<Statement>> 
    ForStatement::getBodyStatements() const
    {
        return m_bodyStatements;
    }

    std::string ForStatement::toString() const
    {
        std::string str("\nKeyword: ");
        str.append(m_keywordToken.raw);
        str.append("\nIndexer: ");
        str.append(m_indexer.raw);
        str.append("\nIdentifier: ");
        str.append(m_identifier.raw);
        str.append("\nBegin body statements:\n");
        for(auto const & statement : m_bodyStatements) {
            str.append(statement->toString());
        }
        str.append("\nEnd body statements.");
        return str;
    }
    Token ForStatement::getIndexer() const
    {
        return m_indexer;
    }
    Token ForStatement::getIdentifier() const
    {
        return m_identifier;
    }
    std::shared_ptr<StatementEvaluator> ForStatement::getEvaluator() const
    {
        return std::make_shared<ForStatementEvaluator>(*this);
    }
}