/// (c) Ben Jones 2019 - present

#pragma once

#include <string>

namespace arrow {
    
    class BuiltInFunctionExpression
    {
      public:
      	virtual std::string getName() const = 0;
    };
}