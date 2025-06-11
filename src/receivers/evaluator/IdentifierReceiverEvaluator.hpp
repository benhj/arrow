/// (c) Ben Jones 2019

#pragma once

#include "ReceiverEvaluator.hpp"
#include "lexer/Token.hpp"
#include "representation/Environment.hpp"

namespace arrow {

    struct IdentifierReceiverEvaluator final : public ReceiverEvaluator {
        IdentifierReceiverEvaluator(Token tok);
        void evaluate(Type incoming, Environment & environment) const override;
      private:
        Token m_tok;
    };

}