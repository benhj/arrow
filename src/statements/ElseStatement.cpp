/// (c) Ben Jones 2019 - present

#include "ElseStatement.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include <utility>

namespace arrow {

    ElseStatement::ElseStatement(long const lineNumber) 
      : Statement(lineNumber)
    {
    }
    ElseStatement & ElseStatement::withToken(Token token)
    {
        m_keywordToken = std::move(token);
        return *this;
    }
    ElseStatement & ElseStatement::withInnerStatement(std::shared_ptr<Statement> innerStatement)
    {
        m_innerStatement = std::move(innerStatement);
        return *this;
    }
    std::shared_ptr<Statement> ElseStatement::getInnerStatement() const
    {
        return m_innerStatement;
    }
    std::string ElseStatement::toString() const
    {
        return ""; // TODO
    }

    std::shared_ptr<StatementEvaluator> ElseStatement::getEvaluator() const
    {
        return nullptr;
    }
}