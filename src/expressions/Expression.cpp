#include "Expression.hpp"

namespace arrow {
    Expression::Expression(long const lineNumber)
      : Node()
      , m_lineNumber(lineNumber)
    {
    }

    long Expression::getLineNumber() const
    {
    	return m_lineNumber;
    }
}