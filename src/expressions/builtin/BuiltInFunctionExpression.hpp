/// (c) Ben Jones 2019 - present

#pragma once

#include "expressions/Expression.hpp"
#include <memory>

namespace arrow {
    
    class BuiltInFunctionExpression
    {
      public:
      	virtual void setExpression(std::shared_ptr<Expression>) = 0;
    };
}