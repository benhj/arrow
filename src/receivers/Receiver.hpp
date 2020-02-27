/// (c) Ben Jones 2019 - present

#pragma once

#include "parser/Node.hpp"
#include "representation/Type.hpp"
#include <memory>
#include <string>

namespace arrow {

    class ReceiverEvaluator;

    class Receiver : public Node
    {
      public:
        Receiver(long const lineNumber);

        /// Returns an Evaluator that can be used to evaluate this
        virtual std::shared_ptr<ReceiverEvaluator> getEvaluator() const;

        /// For debugging purposes
        virtual std::string toString() const = 0;

        long getLineNumber() const;

      protected:
        long m_lineNumber;
    };
}