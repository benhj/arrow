/// (c) Ben Jones 2019

#pragma once

#include "ReceiverEvaluator.hpp"
#include "lexer/Token.hpp"
#include "representation/Cache.hpp"

namespace arrow {

    struct DollarIdentifierReceiverEvaluator : public ReceiverEvaluator {
        DollarIdentifierReceiverEvaluator(Token tok);
        void evaluate(Type incoming, Cache & cache) const override;
      private:
        Token m_tok;
    };

}