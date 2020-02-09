/// (c) Ben Jones 2019 - present

#pragma once

#include "parser/Node.hpp"
#include <memory>
#include <ostream>
#include <string>

namespace arrow {
	
    class StatementEvaluator;
	class Statement : public Node
	{
	  public:
        Statement(long const lineNumber, std::ostream & os) 
          : Node()
          , m_lineNumber(lineNumber)
          , m_os(os)
        {
        }

        /// Returns an Evaluator that can be used to evaluate this
        virtual std::shared_ptr<StatementEvaluator> getEvaluator() const
        {
            return nullptr;
        }

        /// For debugging purposes
	  	virtual std::string toString() const = 0;

	  	long getLineNumber() const
	  	{
	  		return m_lineNumber;
	  	}
	  protected:
	  	long m_lineNumber;
	  	std::ostream & m_os;
	};

}