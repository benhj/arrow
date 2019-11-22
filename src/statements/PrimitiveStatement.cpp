#include "PrimitiveStatement.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include <utility>

namespace jasl {
    
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
        return nullptr;
    }
}