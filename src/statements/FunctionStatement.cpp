/// (c) Ben Jones 2019 - present

#include "FunctionStatement.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include "evaluator/FunctionStatementEvaluator.hpp"
#include <utility>

namespace arrow {

    FunctionStatement::FunctionStatement(long const lineNumber) 
      : Statement(lineNumber)
    {
    }
    FunctionStatement & FunctionStatement::withToken(Token token)
    {
        m_keywordToken = std::move(token);
        return *this;
    }
    FunctionStatement & FunctionStatement::withNameToken(Token token)
    {
        m_nameToken = std::move(token);
        return *this;
    }
    FunctionStatement & FunctionStatement::withReturnIdentifierToken(Token token)
    {
        m_returnIdentifier = std::move(token);
        return *this;
    }
    FunctionStatement & 
    FunctionStatement::withExpressionCollection(std::shared_ptr<Expression> expression)
    {
        m_expressionCollection = std::move(expression);
        return *this;
    }
    FunctionStatement & FunctionStatement::withInnerStatement(std::shared_ptr<Statement> innerStatement)
    {
        m_innerStatement = std::move(innerStatement);
        return *this;
    }

    Token FunctionStatement::getName() const
    {
        return m_nameToken;
    }
    std::shared_ptr<Expression> FunctionStatement::getExpressionCollection() const
    {
        return m_expressionCollection;
    }
    Token FunctionStatement::getReturnIdentifier() const
    {
        return m_returnIdentifier;
    }
    std::shared_ptr<Statement> FunctionStatement::getInnerStatement() const
    {
        return m_innerStatement;
    }

    std::string FunctionStatement::toString() const
    {
        return ""; // TODO
    }

    std::shared_ptr<StatementEvaluator> FunctionStatement::getEvaluator() const
    {
        return std::make_shared<FunctionStatementEvaluator>(*this);
    }
}