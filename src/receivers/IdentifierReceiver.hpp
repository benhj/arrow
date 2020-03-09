/// (c) Ben Jones 2019 - present

#pragma once

#include "Receiver.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace arrow {
    
    class IdentifierReceiver : public Receiver
    {
      public:
        IdentifierReceiver(long const lineNumber);
        std::shared_ptr<ReceiverEvaluator> getEvaluator() const override;
        IdentifierReceiver & withIdentifierToken(Token identifier);
        Token const & getIdentifierToken() const;
        std::string toString() const override;
      private:
        Token m_identifier;
    };

}