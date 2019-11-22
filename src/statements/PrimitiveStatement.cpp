#include "PrimitiveStatement.hpp"
#include "evaluator/PrimitiveStatementEvaluator.hpp"
#include <utility>

namespace arrow {
    
    PrimitiveStatement::PrimitiveStatement(ArrowStatement arrowStatement) 
      : Statement()
      , m_arrowStatement(std::move(arrowStatement))
    {
    }

    std::string PrimitiveStatement::toString() const
    {
        return m_arrowStatement.toString();
    }

    std::shared_ptr<StatementEvaluator> PrimitiveStatement::getEvaluator() const
    {
        return std::make_shared<PrimitiveStatementEvaluator>(*this);
    }

    PrimitiveStatement const & PrimitiveStatement::withToken(Token token) const
    {
        m_arrowStatement.withToken(std::move(token));
        return *this;
    }

    Token PrimitiveStatement::getToken() const
    {
        return m_arrowStatement.getToken();
    }

    std::shared_ptr<Expression> PrimitiveStatement::getExpression() const
    {
        return m_arrowStatement.getExpression();
    }

    Token PrimitiveStatement::getIdentifier() const
    {
        return m_arrowStatement.getIdentifier();
    }
}