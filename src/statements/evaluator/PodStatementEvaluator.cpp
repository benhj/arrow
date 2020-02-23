/// (c) Ben Jones 2020 - present

#include "PodStatementEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

namespace arrow {
    PodStatementEvaluator::PodStatementEvaluator(PodStatement statement)
      : m_statement(std::move(statement))
    {
    }
    StatementResult PodStatementEvaluator::evaluate(Environment &) const
    {
        return StatementResult::Continue;
    }
}