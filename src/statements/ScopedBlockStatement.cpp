/// (c) Ben Jones 2019 - present

#include "ScopedBlockStatement.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include "evaluator/ScopedBlockStatementEvaluator.hpp"
#include <utility>

namespace arrow {

    ScopedBlockStatement::ScopedBlockStatement(long const lineNumber)
    : Statement(lineNumber)
    {
    }
    void ScopedBlockStatement::addBodyStatement(std::shared_ptr<Statement> bodyStatement)
    {
        m_bodyStatements.emplace_back(std::move(bodyStatement));
    }

    std::string ScopedBlockStatement::toString() const
    {
        return ""; // TODO
    }
    std::vector<std::shared_ptr<Statement>> const & 
    ScopedBlockStatement::getBodyStatements() const
    {
        return m_bodyStatements;
    }
    std::shared_ptr<StatementEvaluator> ScopedBlockStatement::getEvaluator() const
    {
        return std::make_shared<ScopedBlockStatementEvaluator>(*this);
    }
}