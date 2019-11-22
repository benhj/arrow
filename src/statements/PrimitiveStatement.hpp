#pragma once

#include "ArrowStatement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>

namespace jasl {
    
    class PrimitiveStatement : public Statement
    {
      public:
        PrimitiveStatement(ArrowStatement arrowStatement);
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
      private:
        ArrowStatement m_arrowStatement;
    };

}