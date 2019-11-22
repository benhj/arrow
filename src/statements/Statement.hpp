#pragma once

#include "parser/Node.hpp"
#include <memory>
#include <string>

namespace jasl {
	
    class StatementEvaluator;
	class Statement : public Node
	{
	  public:
        Statement() : Node() {}

        /// Returns an Evaluator that can be used to evaluate this
        virtual std::shared_ptr<StatementEvaluator> getEvaluator() const = 0;

        /// For debugging purposes
	  	virtual std::string toString() const = 0;
	};

}