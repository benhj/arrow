/// (c) Ben Jones 2019 - present

#pragma once

#include "Receiver.hpp"
#include "expressions/Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace arrow {
    
    class DollarIdentifierReceiver final : public Receiver
    {
      public:
        DollarIdentifierReceiver(long const lineNumber);
        std::shared_ptr<ReceiverEvaluator> getEvaluator() const override;
        DollarIdentifierReceiver & withExpression(std::shared_ptr<Expression>);
        std::shared_ptr<Expression> const & getExpression() const;
        std::string toString() const override;
      private:
        std::shared_ptr<Expression> m_expression;
    };

}