/// (c) Ben Jones 2020 - present

#pragma once

#include "ExpressionEvaluator.hpp"
#include "lexer/Token.hpp"
#include "representation/Environment.hpp"

namespace arrow {

    struct NegativeNumberEvaluator : public ExpressionEvaluator {
        NegativeNumberEvaluator(std::shared_ptr<ExpressionEvaluator> innerEvaluator,
        						long const lineNumber);
        Type evaluate(Environment & environment) const override;
      private:
        std::shared_ptr<ExpressionEvaluator> m_innerEvaluator;
        long m_lineNumber;
    };

}