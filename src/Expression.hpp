#pragma once
#include <string>

namespace jasl {
    
    class Expression
    {
      public:
        Expression(){}
        virtual std::string toString() const = 0;
    };


}