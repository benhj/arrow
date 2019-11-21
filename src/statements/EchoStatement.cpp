#include "EchoStatement.hpp"
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
}