/// (c) Ben Jones 2019 - present

#pragma once

#include "expressions/Expression.hpp"
#include <string>
#include <memory>

namespace arrow {
    
    class BuiltInFunctionExpressionBuilder
    {
      public:
      	virtual std::string getName() const = 0;
      	virtual std::shared_ptr<Expression> build(long const lineNumber) const = 0;
    };
}