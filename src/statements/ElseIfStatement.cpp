/// (c) Ben Jones 2019 - present

#include "ElseIfStatement.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include <utility>

namespace arrow {

    ElseIfStatement::ElseIfStatement(long const lineNumber)
    : Statement(lineNumber)
    {
    }
    ElseIfStatement & ElseIfStatement::withToken(Token token)
    {
        m_keywordToken = std::move(token);
        return *this;
    }
    ElseIfStatement & ElseIfStatement::withExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
        return *this;
    }
    std::shared_ptr<Expression> ElseIfStatement::getExpression() const
    {
        return m_expression;
    }
    ElseIfStatement & ElseIfStatement::withInnerStatement(std::shared_ptr<Statement> innerStatement)
    {
        m_innerStatement = std::move(innerStatement);
        return *this;
    }
    std::shared_ptr<Statement> ElseIfStatement::getInnerStatement() const
    {
        return m_innerStatement;
    }

    std::string ElseIfStatement::toString() const
    {
        return ""; // TODO
    }

    std::shared_ptr<StatementEvaluator> ElseIfStatement::getEvaluator() const
    {
        return nullptr;
    }
}