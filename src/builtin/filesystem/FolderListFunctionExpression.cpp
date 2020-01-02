/// (c) Ben Jones 2019 - present

#include "FolderListFunctionExpression.hpp"
#include "evaluator/FolderListFunctionExpressionEvaluator.hpp"

namespace arrow {

    FolderListFunctionExpression::FolderListFunctionExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> FolderListFunctionExpression::getEvaluator() const
    {
        return std::make_shared<FolderListFunctionExpressionEvaluator>(*this);
    }

    void FolderListFunctionExpression::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression>
    FolderListFunctionExpression::getExpression() const
    {
        return m_expression;
    }

    std::string FolderListFunctionExpression::toString() const
    {
        return ""; // todo
    }
}