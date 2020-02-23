/// (c) Ben Jones 2019 - present

#include "SimpleArrowStatement.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include "evaluator/SimpleArrowStatementEvaluator.hpp"
#include <utility>

namespace arrow {
    
    SimpleArrowStatement::SimpleArrowStatement(long const lineNumber)
    : Statement(lineNumber)
    {
    }

    SimpleArrowStatement &
    SimpleArrowStatement::withExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
        return *this;
    }
    SimpleArrowStatement & SimpleArrowStatement::withIdentifier(std::shared_ptr<Receiver> identifier)
    {
        m_identifier = std::move(identifier);
        return *this;
    }
    SimpleArrowStatement &
    SimpleArrowStatement::withIndexExpression(std::shared_ptr<Expression> expression)
    {
        m_indexExpression = std::move(expression);
        return *this;
    }

    std::shared_ptr<Expression> SimpleArrowStatement::getExpression() const
    {
        return m_expression;
    }
    std::shared_ptr<Expression> SimpleArrowStatement::getIndexExpression() const
    {
        return m_indexExpression;
    }

    std::shared_ptr<Receiver> SimpleArrowStatement::getIdentifier() const
    {
        return m_identifier;
    }

    std::string SimpleArrowStatement::toString() const
    {
        return ""; // TODO
    }

    std::shared_ptr<StatementEvaluator> SimpleArrowStatement::getEvaluator() const
    {
        return std::make_shared<SimpleArrowStatementEvaluator>(*this);
    }
}