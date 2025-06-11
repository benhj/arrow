/// (c) Ben Jones 2019

#pragma once

#include "receivers/evaluator/ReceiverEvaluator.hpp"
#include "builtin/filesystem/FileReceiver.hpp"
#include "representation/Environment.hpp"
#include "representation/Type.hpp"

namespace arrow {
    class FileReceiverEvaluator final : public ReceiverEvaluator
    {
      public:
        explicit FileReceiverEvaluator(FileReceiver receiver);
         void evaluate(Type incoming, Environment & environment) const override;
      private:
        FileReceiver m_receiver;
    };
}