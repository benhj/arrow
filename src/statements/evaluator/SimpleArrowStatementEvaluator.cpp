/// (c) Ben Jones 2019 - present

#include "SimpleArrowStatementEvaluator.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "expressions/IndexExpression.hpp"
#include "receivers/evaluator/ReceiverEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

namespace arrow {

    SimpleArrowStatementEvaluator::SimpleArrowStatementEvaluator(SimpleArrowStatement statement)
      : m_statement(std::move(statement))
    {
    }
    StatementResult SimpleArrowStatementEvaluator::evaluate(Environment & environment) const
    {
        auto const expression = m_statement.getExpression();
        auto evaluated = expression->getEvaluator()->evaluate(environment);
        auto receiver = m_statement.getIdentifier();
        auto evaluator = receiver->getEvaluator();
        evaluator->evaluate(std::move(evaluated), environment);
        return StatementResult::Continue;
    }
}