/// (c) Ben Jones 2020 - present

#pragma once

#include "expressions/Expression.hpp"
#include "receivers/Receiver.hpp"
#include "receivers/evaluator/ReceiverEvaluator.hpp"
#include "lexer/Token.hpp"
#include <memory>
#include <vector>

namespace arrow {
    class FileReceiver : public Receiver
    {
      public:
        FileReceiver(long const lineNumber);
        void setExpression(std::shared_ptr<Expression> expression);
        std::shared_ptr<Expression> getExpression() const;
        std::shared_ptr<ReceiverEvaluator> getEvaluator() const override;
        std::string toString() const override;
      private:
        std::shared_ptr<Expression> m_expression;
    };
}