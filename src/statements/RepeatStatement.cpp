/// (c) Ben Jones 2019 - present

#include "RepeatStatement.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include "evaluator/RepeatStatementEvaluator.hpp"
#include <utility>

namespace arrow {

    RepeatStatement::RepeatStatement(long const lineNumber)
    : Statement(lineNumber)
    {
    }
    RepeatStatement & RepeatStatement::withToken(Token token)
    {
        m_keywordToken = std::move(token);
        return *this;
    }
    RepeatStatement & RepeatStatement::withExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
        return *this;
    }
    RepeatStatement & RepeatStatement::withInnerStatement(std::shared_ptr<Statement> innerStatement)
    {
        m_innerStatement = std::move(innerStatement);
        return *this;
    }

    std::string RepeatStatement::toString() const
    {
        return ""; // TODO
    }
    std::shared_ptr<Expression> RepeatStatement::getExpression() const
    {
        return m_expression;
    }
    std::shared_ptr<Statement> RepeatStatement::getInnerStatement() const
    {
        return m_innerStatement;
    }

    std::shared_ptr<StatementEvaluator> RepeatStatement::getEvaluator() const
    {
        return std::make_shared<RepeatStatementEvaluator>(*this);
    }
}