/// (c) Ben Jones 2019 - present

#include "ElseIfStatement.hpp"
#include "ElseStatement.hpp"
#include "IfStatement.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include "evaluator/IfStatementEvaluator.hpp"
#include <utility>

namespace arrow {

    IfStatement::IfStatement(long const lineNumber)
    : Statement(lineNumber)
    {
    }
    IfStatement & IfStatement::withToken(Token token)
    {
        m_keywordToken = std::move(token);
        return *this;
    }
    IfStatement & IfStatement::withExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
        return *this;
    }
    std::shared_ptr<Expression> const & IfStatement::getExpression() const
    {
        return m_expression;
    }
    IfStatement & IfStatement::withInnerStatement(std::shared_ptr<Statement> innerStatement)
    {
        m_innerStatement = std::move(innerStatement);
        return *this;
    }
    void IfStatement::addElseIfPart(std::shared_ptr<ElseIfStatement> elseIfPart)
    {
        m_elseIfParts.emplace_back(std::move(elseIfPart));
    }
    IfStatement & IfStatement::withElsePart(std::shared_ptr<ElseStatement> elsePart)
    {
        m_elsePart = std::move(elsePart);
        return *this;
    }
    std::shared_ptr<Statement> const & IfStatement::getInnerStatement() const
    {
        return m_innerStatement;
    }

    std::vector<std::shared_ptr<ElseIfStatement>> const & 
    IfStatement::getElseIfParts() const
    {
        return m_elseIfParts;
    }

    std::shared_ptr<ElseStatement> const & IfStatement::getElsePart() const
    {
        return m_elsePart;
    }

    std::string IfStatement::toString() const
    {
        return ""; // TODO
    }

    std::shared_ptr<StatementEvaluator> IfStatement::getEvaluator() const
    {
        return std::make_shared<IfStatementEvaluator>(*this);
    }
}