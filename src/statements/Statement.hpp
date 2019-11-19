#pragma once

#include "parser/Node.hpp"
#include <string>

namespace jasl {
	
	class Statement : public Node
	{
	  public:
        Statement() : Node() {}
	  	virtual std::string toString() const = 0;
	};

}