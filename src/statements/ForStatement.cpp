/// (c) Ben Jones 2019 - present

#include "ForStatement.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include "evaluator/ForStatementEvaluator.hpp"
#include <utility>

namespace arrow {

    ForStatement::ForStatement(long const lineNumber, std::ostream & os)
    : Statement(lineNumber, os)
    {
    }
    ForStatement & ForStatement::withToken(Token token)
    {
        m_keywordToken = std::move(token);
        return *this;
    }
    ForStatement & ForStatement::addIndexer(Token token)
    {
        m_indices.push_back(token);
        return *this;
    }
    ForStatement & ForStatement::withIdentifier(Token token)
    {
        m_identifier = std::move(token);
        return *this;
    }
    ForStatement & ForStatement::withInnerStatement(std::shared_ptr<Statement> innerStatement)
    {
        m_innerStatement= std::move(innerStatement);
        return *this;
    }
    std::shared_ptr<Statement> ForStatement::getInnerStatement() const
    {
        return m_innerStatement;
    }

    std::string ForStatement::toString() const
    {
        return ""; // TODO
    }
    std::vector<Token> ForStatement::getIndices() const
    {
        return m_indices;
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