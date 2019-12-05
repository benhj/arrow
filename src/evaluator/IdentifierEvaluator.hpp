/// (c) Ben Jones 2019

#pragma once

#include "ExpressionEvaluator.hpp"
#include "lexer/Token.hpp"
#include "representation/Cache.hpp"

namespace arrow {

    struct IdentifierEvaluator : public ExpressionEvaluator {
        IdentifierEvaluator(Token tok);
        Type evaluate(Cache & cache) const override;
      private:
        Token m_tok;
    };

}