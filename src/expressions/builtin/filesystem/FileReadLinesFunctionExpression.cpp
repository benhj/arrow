/// (c) Ben Jones 2019 - present

#include "FileReadLinesFunctionExpression.hpp"
#include "evaluator/builtin/filesystem/FileReadLinesFunctionExpressionEvaluator.hpp"

namespace arrow {

    FileReadLinesFunctionExpression::FileReadLinesFunctionExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> FileReadLinesFunctionExpression::getEvaluator() const
    {
        return std::make_shared<FileReadLinesFunctionExpressionEvaluator>(*this);
    }

    void FileReadLinesFunctionExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression>
    FileReadLinesFunctionExpression::getExpression() const
    {
        return m_expression;
    }

    std::string FileReadLinesFunctionExpression::toString() const
    {
        return ""; // todo
    }
}