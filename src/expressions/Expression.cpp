#include "Expression.hpp"
#include "parser/LanguageException.hpp"

namespace arrow {
    Expression::Expression(long const lineNumber)
      : Node()
      , m_lineNumber(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> Expression::getEvaluator() const
    {
        throw LanguageException("Cannot evaluate expression", m_lineNumber);
    }

    long Expression::getLineNumber() const
    {
        return m_lineNumber;
    }
}