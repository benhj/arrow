/// (c) Ben Jones 2019 - present

#pragma once

#include "Receiver.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace arrow {
    
    class DollarIdentifierReceiver : public Receiver
    {
      public:
        DollarIdentifierReceiver(long const lineNumber);
        std::shared_ptr<ReceiverEvaluator> getEvaluator() const override;
        DollarIdentifierReceiver & withIdentifierToken(Token identifier);
        Token getIdentifierToken() const;
        std::string toString() const override;
      private:
        Token m_identifier;
    };

}