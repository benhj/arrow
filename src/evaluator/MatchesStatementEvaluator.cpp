#include "MatchesStatementEvaluator.hpp"
#include <utility>
#include <vector>

namespace arrow {
    MatchesStatementEvaluator::MatchesStatementEvaluator(MatchesStatement statement)
      : m_statement(std::move(statement))
    {
    }
    void MatchesStatementEvaluator::evaluate(Cache & cache) const 
    {
        // TODO
    }
}