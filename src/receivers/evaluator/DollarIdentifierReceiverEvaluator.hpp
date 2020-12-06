/// (c) Ben Jones 2019 - present

#pragma once

#include "ReceiverEvaluator.hpp"
#include "expressions/Expression.hpp"
#include "representation/Environment.hpp"

namespace arrow {

    class IdentifierExpression;

    struct DollarIdentifierReceiverEvaluator : public ReceiverEvaluator {
        explicit DollarIdentifierReceiverEvaluator(std::shared_ptr<Expression> expression);
        void evaluate(Type incoming, Environment & environment) const override;
      private:
        std::shared_ptr<Expression> m_expression;

        void handleIdentifierExpression(Type evaluated,
                                        Environment & environment,
                                        IdentifierExpression * expr) const;
    };

}