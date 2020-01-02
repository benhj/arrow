/// (c) Ben Jones 2019 - present

#include "FolderListRecursiveFunctionExpression.hpp"
#include "evaluator/FolderListRecursiveFunctionExpressionEvaluator.hpp"

namespace arrow {

    FolderListRecursiveFunctionExpression::FolderListRecursiveFunctionExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> FolderListRecursiveFunctionExpression::getEvaluator() const
    {
        return std::make_shared<FolderListRecursiveFunctionExpressionEvaluator>(*this);
    }

    void FolderListRecursiveFunctionExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression>
    FolderListRecursiveFunctionExpression::getExpression() const
    {
        return m_expression;
    }

    std::string FolderListRecursiveFunctionExpression::toString() const
    {
        return ""; // todo
    }
}