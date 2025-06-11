/// (c) Ben Jones 2019 - present

#pragma once

#include "ReceiverEvaluator.hpp"
#include "expressions/Expression.hpp"
#include "representation/Environment.hpp"

namespace arrow {

    class IdentifierExpression;
    class IndexExpression;

    struct DollarIdentifierReceiverEvaluator final : public ReceiverEvaluator {
        explicit DollarIdentifierReceiverEvaluator(std::shared_ptr<Expression> expression);
        void evaluate(Type incoming, Environment & environment) const override;
      private:
        std::shared_ptr<Expression> m_expression;

        /// Likely identifies a vector
        void handleIdentifierExpression(Type evaluated,
                                        Environment & environment,
                                        IdentifierExpression * expr) const;

        /// An index expression could represent a map or a pod
        void handleIndexExpression(Type evaluated,
                                   Environment & environment,
                                   IndexExpression * expr) const;
    };

}