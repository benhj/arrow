/// (c) Ben Jones 2019 - present

#include "WhileStatement.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include "evaluator/WhileStatementEvaluator.hpp"
#include <utility>

namespace arrow {

    WhileStatement::WhileStatement(long const lineNumber)
    : Statement(lineNumber)
    {
    }
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
    WhileStatement & WhileStatement::withInnerStatement(std::shared_ptr<Statement> innerStatement)
    {
        m_innerStatement = std::move(innerStatement);
        return *this;
    }
    std::shared_ptr<Statement> const & WhileStatement::getInnerStatement() const
    {
        return m_innerStatement;
    }

    std::string WhileStatement::toString() const
    {
        return ""; // TODO
    }
    std::shared_ptr<Expression> const & WhileStatement::getExpression() const
    {
        return m_expression;
    }
    std::shared_ptr<StatementEvaluator> WhileStatement::getEvaluator() const
    {
        return std::make_shared<WhileStatementEvaluator>(*this);
    }
}