#pragma once
#include "DecayType.hpp"
#include "parser/Node.hpp"
#include "representation/Type.hpp"
#include <string>

namespace arrow {

    class ExpressionEvaluator;

    class Expression : public Node
    {
      public:
        Expression(long const lineNumber);

        /// Returns an Evaluator that can be used to evaluate this
        virtual std::shared_ptr<ExpressionEvaluator> getEvaluator() const = 0;

        /// During expression evaluation, we need
        /// to know the overaching type that it decays to
        virtual DecayType decayType() const = 0;

        /// For debugging purposes
        virtual std::string toString() const = 0;

      protected:
        long m_lineNumber;
    };
}