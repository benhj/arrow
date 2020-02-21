/// (c) Ben Jones 2019

#pragma once

#include "ExpressionEvaluator.hpp"
#include "lexer/Token.hpp"
#include "representation/Environment.hpp"

namespace arrow {

    struct IdentifierEvaluator : public ExpressionEvaluator {
        IdentifierEvaluator(Token tok);
        Type evaluate(Environment & environment) const override;
      private:
        Token m_tok;
    };

}