/// (c) Ben Jones 2019 - present

#include "SingleExpressionStatement.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include <utility>

namespace arrow {

    SingleExpressionStatement::SingleExpressionStatement(long const lineNumber) 
      : Statement(lineNumber)
    {
    }
    SingleExpressionStatement& 
    SingleExpressionStatement::withExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
        return *this;
    }
    std::shared_ptr<Expression> SingleExpressionStatement::getExpression() const
    {
        return m_expression;
    }

    std::string SingleExpressionStatement::toString() const
    {
        return ""; // todo
    }

    std::shared_ptr<StatementEvaluator> SingleExpressionStatement::getEvaluator() const
    {
        struct SingleExpressionStatementEvaluator : public StatementEvaluator
        {
            SingleExpressionStatementEvaluator(SingleExpressionStatement statement)
              : m_statement(std::move(statement))
            {}
            bool evaluate(Cache & cache) const override
            {
                auto const expression = m_statement.getExpression();
                (void)expression->getEvaluator()->evaluate(cache);
                return true;
            }
          private:
            SingleExpressionStatement m_statement;
        };
        return std::make_shared<SingleExpressionStatementEvaluator>(*this);
    }
}