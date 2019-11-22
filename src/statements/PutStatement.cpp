#include "PutStatement.hpp"
#include "evaluator/PutStatementEvaluator.hpp"
#include <utility>

namespace arrow {
    
    PutStatement::PutStatement(ArrowStatement arrowStatement) 
      : Statement()
      , m_arrowStatement(std::move(arrowStatement))
    {
    }

    std::string PutStatement::toString() const
    {
        return m_arrowStatement.toString();
    }

    std::shared_ptr<StatementEvaluator> PutStatement::getEvaluator() const
    {
        return std::make_shared<PutStatementEvaluator>(*this);
    }

    PutStatement const & PutStatement::withToken(Token token) const
    {
        m_arrowStatement.withToken(std::move(token));
        return *this;
    }

    Token PutStatement::getToken() const
    {
        return m_arrowStatement.getToken();
    }

    std::shared_ptr<Expression> PutStatement::getExpression() const
    {
        return m_arrowStatement.getExpression();
    }

    Token PutStatement::getIdentifier() const
    {
        return m_arrowStatement.getIdentifier();
    }

    ArrowStatement PutStatement::getArrowStatement() const
    {
        return m_arrowStatement;
    }
}