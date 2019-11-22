#include "EchoStatement.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include <utility>

namespace jasl {

    EchoStatement::EchoStatement(std::shared_ptr<ArrowlessStatement> statement)
    : Statement()
    , m_statement(std::move(statement))
    {

    }
    std::string EchoStatement::toString() const
    {
        return m_statement->toString();
    }

    std::shared_ptr<StatementEvaluator> EchoStatement::getEvaluator() const
    {
        return nullptr;
    }
}