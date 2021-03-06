/// (c) Ben Jones 2019 - present

#pragma once

#include "parser/Node.hpp"
#include "representation/Type.hpp"
#include <memory>
#include <string>

namespace arrow {

    class ExpressionEvaluator;

    class Expression : public Node
    {
      public:
        Expression(long const lineNumber);

        /// Returns an Evaluator that can be used to evaluate this
        virtual std::shared_ptr<ExpressionEvaluator> getEvaluator() const;

        /// For debugging purposes
        virtual std::string toString() const = 0;

        // A plain char string indicating the 'type of this'
        virtual char const * getTypeString() const;

        long getLineNumber() const;

      protected:
        long m_lineNumber;
    };
}