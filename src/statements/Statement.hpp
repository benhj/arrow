#pragma once

#include "parser/Node.hpp"
#include <memory>
#include <string>

namespace arrow {
	
    class StatementEvaluator;
	class Statement : public Node
	{
	  public:
        Statement(long const lineNumber) 
          : Node()
          , m_lineNumber(lineNumber)
        {
        }

        /// Returns an Evaluator that can be used to evaluate this
        virtual std::shared_ptr<StatementEvaluator> getEvaluator() const = 0;

        /// For debugging purposes
	  	virtual std::string toString() const = 0;

	  	long getLineNumber() const
	  	{
	  		return m_lineNumber;
	  	}

	  protected:
	  	long m_lineNumber;
	};

}