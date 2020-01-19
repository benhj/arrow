/// (c) Ben Jones 2020 - present

#pragma once

#include "ExpressionParser.hpp"
#include "TokenManager.hpp"
#include "receivers/Receiver.hpp"
#include "lexer/Token.hpp"
#include <memory>

namespace arrow {

    class ReceiverParser
    {
      public:
        ReceiverParser(TokenManager & tm);
        ReceiverParser() = delete;
        std::shared_ptr<Receiver> parseReceiver();
        std::shared_ptr<Receiver> parseIdentifierReceiver();
        std::shared_ptr<Receiver> parseDollarIdentifierReceiver();
        std::shared_ptr<Receiver> parseArrayAccessorReceiver();
        std::shared_ptr<Receiver> parseFileReceiver();
      private:
        TokenManager & m_tm;
        ExpressionParser m_ep;
    };
}
