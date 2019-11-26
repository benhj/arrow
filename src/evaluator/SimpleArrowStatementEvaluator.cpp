#include "SimpleArrowStatementEvaluator.hpp"
#include "ExpressionEvaluator.hpp"
#include "expressions/IndexExpression.hpp"
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
        auto const identifier = m_statement.getIdentifier();

        // first check if the target is an index expression rather
        // than an identifier
        auto indexExpression = m_statement.getIndexExpression();
        if(indexExpression) {
            auto casted = dynamic_cast<IndexExpression*>(indexExpression.get());
            auto containerIdentifier = casted->getIdentifierToken();
            auto exp = casted->getIndexExpression();
            auto indexEval = exp->getEvaluator()->evaluate(cache);
            if(indexEval.m_descriptor != TypeDescriptor::Int) {
                std::string error("Bad expression for index at line ");
                error.append(std::to_string(casted->getIdentifierToken().lineNumber));
                throw std::runtime_error(error);
            }
            auto index = std::get<int64_t>(indexEval.m_variantType);
            try {
                cache.setElementInContainer(containerIdentifier, index, evaluated);
            } catch (...) {
                std::string error("Index too big at line ");
                error.append(std::to_string(m_statement.getIdentifier().lineNumber));
                throw std::runtime_error(error);
            }
        } else {
            cache.add(identifier, evaluated);
        }
    }
}