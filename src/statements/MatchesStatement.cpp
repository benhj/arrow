/// (c) Ben Jones 2019 - present

#include "MatchesStatement.hpp"
#include "evaluator/MatchesStatementEvaluator.hpp"
#include <utility>

namespace arrow {
    
    MatchesStatement::MatchesStatement(long const lineNumber) 
      : Statement(lineNumber)
    {
    }

    MatchesStatement &
    MatchesStatement::withLeftExpression(std::shared_ptr<Expression> expression)
    {
        m_left = std::move(expression);
        return *this;
    }
    MatchesStatement & MatchesStatement::withIdentifier(Token identifier)
    {
        m_identifier = std::move(identifier);
        return *this;
    }    
    MatchesStatement &
    MatchesStatement::withRightExpression(std::shared_ptr<Expression> expression)
    {
        m_right = std::move(expression);
        return *this;
    }

    std::shared_ptr<Expression> MatchesStatement::getLeftExpression() const
    {
        return m_left;
    }

    std::shared_ptr<Expression> MatchesStatement::getRightExpression() const
    {
        return m_right;
    }

    Token MatchesStatement::getIdentifier() const
    {
        return m_identifier;
    }

    std::string MatchesStatement::toString() const
    {
        return ""; // TODO
    }

    std::shared_ptr<StatementEvaluator> MatchesStatement::getEvaluator() const
    {
        return std::make_shared<MatchesStatementEvaluator>(*this);
    }
}