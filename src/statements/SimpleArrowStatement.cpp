#include "SimpleArrowStatement.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include "evaluator/SimpleArrowStatementEvaluator.hpp"
#include <utility>

namespace arrow {
    
    SimpleArrowStatement::SimpleArrowStatement() : Statement() {}

    SimpleArrowStatement &
    SimpleArrowStatement::withExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
        return *this;
    }
    SimpleArrowStatement & SimpleArrowStatement::withIdentifier(Token identifier)
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

    Token SimpleArrowStatement::getIdentifier() const
    {
        return m_identifier;
    }

    std::string SimpleArrowStatement::toString() const
    {
        std::string str("\nExpression: ");
        str.append(m_expression->toString());
        str.append("\nIdentifier: ");
        str.append(m_identifier.raw);
        return str;
    }

    std::shared_ptr<StatementEvaluator> SimpleArrowStatement::getEvaluator() const
    {
        return std::make_shared<SimpleArrowStatementEvaluator>(*this);
    }
}