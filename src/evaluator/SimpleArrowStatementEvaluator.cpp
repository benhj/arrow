#include "SimpleArrowStatementEvaluator.hpp"
#include "ExpressionEvaluator.hpp"
#include <utility>

namespace arrow {

    SimpleArrowStatementEvaluator::SimpleArrowStatementEvaluator(SimpleArrowStatement statement)
      : m_statement(std::move(statement))
    {
    }
    void SimpleArrowStatementEvaluator::evaluate(Cache & cache) const
    {
        auto const expression = m_statement.getExpression();
        auto evaluated = expression->getEvaluator()->evaluate(cache);
        auto const identifier = m_statement.getIdentifier().raw;
        cache.add(identifier, evaluated);
    }
}