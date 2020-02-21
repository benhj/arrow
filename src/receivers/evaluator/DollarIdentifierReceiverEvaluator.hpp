/// (c) Ben Jones 2019

#pragma once

#include "ReceiverEvaluator.hpp"
#include "lexer/Token.hpp"
#include "representation/Environment.hpp"

namespace arrow {

    struct DollarIdentifierReceiverEvaluator : public ReceiverEvaluator {
        DollarIdentifierReceiverEvaluator(Token tok);
        void evaluate(Type incoming, Environment & cache) const override;
      private:
        Token m_tok;
    };

}