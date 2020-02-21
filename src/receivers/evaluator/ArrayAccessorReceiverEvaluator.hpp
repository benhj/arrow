/// (c) Ben Jones 2019

#pragma once

#include "ReceiverEvaluator.hpp"
#include "expressions/Expression.hpp"
#include "lexer/Token.hpp"
#include "representation/Environment.hpp"

namespace arrow {

    struct ArrayAccessorReceiverEvaluator : public ReceiverEvaluator {
        ArrayAccessorReceiverEvaluator(Token tok, std::shared_ptr<Expression> expression);
        void evaluate(Type incoming, Environment & environment) const override;
      private:
        Token m_tok;
        std::shared_ptr<Expression> m_expression;
    };

}