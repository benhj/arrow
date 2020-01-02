/// (c) Ben Jones 2019 - present

#include "FileReadBytesFunctionExpression.hpp"
#include "evaluator/FileReadBytesFunctionExpressionEvaluator.hpp"

namespace arrow {

    FileReadBytesFunctionExpression::FileReadBytesFunctionExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> FileReadBytesFunctionExpression::getEvaluator() const
    {
        return std::make_shared<FileReadBytesFunctionExpressionEvaluator>(*this);
    }

    void FileReadBytesFunctionExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression>
    FileReadBytesFunctionExpression::getExpression() const
    {
        return m_expression;
    }

    std::string FileReadBytesFunctionExpression::toString() const
    {
        return ""; // todo
    }
}