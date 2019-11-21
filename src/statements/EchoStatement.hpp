#pragma once

#include "Statement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>
#include <utility>

namespace jasl {
    
    class EchoStatement : public Statement
    {
      public:
        EchoStatement(std::shared_ptr<ArrowlessStatement> statement)
        : Statement()
        , m_statement(std::move(statement))
        {

        }
        std::string toString() const override
        {
            return m_statement->toString();
        }
        std::shared_ptr<ArrowlessStatement> m_statement;
    };

}