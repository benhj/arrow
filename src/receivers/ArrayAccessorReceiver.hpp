/// (c) Ben Jones 2019 - present

#pragma once

#include "Receiver.hpp"
#include "expressions/Expression.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace arrow {
    
    class ArrayAccessorReceiver : public Receiver
    {
      public:
        ArrayAccessorReceiver(long const lineNumber);
        std::shared_ptr<ReceiverEvaluator> getEvaluator() const override;
        ArrayAccessorReceiver & withIdentifierToken(Token identifier);
        ArrayAccessorReceiver & withExpression(std::shared_ptr<Expression>);
        Token getIdentifierToken() const;
        std::shared_ptr<Expression> getExpression() const;
        std::string toString() const override;
      private:
        Token m_identifier;
        std::shared_ptr<Expression> m_expression;
    };

}