#pragma once

#include "ExpressionEvaluator.hpp"
#include "lexer/Token.hpp"
#include "representation/Cache.hpp"

namespace jasl {

    struct IdentifierEvaluator : public ExpressionEvaluator {
        IdentifierEvaluator(Token tok);
        Type evaluate(Cache & cache) const override;
      private:
        Token m_tok;
    };

}