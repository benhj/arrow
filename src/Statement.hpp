#pragma once

#include <string>

namespace jasl {
	
	class Statement
	{
	  public:
	  	virtual std::string toString() const = 0;
	};

}