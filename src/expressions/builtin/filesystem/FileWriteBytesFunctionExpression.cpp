/// (c) Ben Jones 2019 - present

#include "FileWriteBytesFunctionExpression.hpp"
#include "evaluator/builtin/filesystem/FileWriteBytesFunctionExpressionEvaluator.hpp"

namespace arrow {

    FileWriteBytesFunctionExpression::FileWriteBytesFunctionExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> FileWriteBytesFunctionExpression::getEvaluator() const
    {
        return std::make_shared<FileWriteBytesFunctionExpressionEvaluator>(*this);
    }

    void FileWriteBytesFunctionExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression>
    FileWriteBytesFunctionExpression::getExpression() const
    {
        return m_expression;
    }

    std::string FileWriteBytesFunctionExpression::toString() const
    {
        return ""; // todo
    }
}