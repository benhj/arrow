/// (c) Ben Jones 2019 - present

#include "ArrowlessStatement.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include <utility>

namespace arrow {

    ArrowlessStatement::ArrowlessStatement(long const lineNumber) 
      : Statement(lineNumber)
    {
    }
    ArrowlessStatement & ArrowlessStatement::withToken(Token token)
    {
        m_keywordToken = std::move(token);
        return *this;
    }
    ArrowlessStatement& 
    ArrowlessStatement::withExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
        return *this;
    }
    Token ArrowlessStatement::getToken() const
    {
        return m_keywordToken;
    }
    std::shared_ptr<Expression> ArrowlessStatement::getExpression() const
    {
        return m_expression;
    }

    std::string ArrowlessStatement::toString() const
    {
        std::string str("\nKeyword: ");
        str.append(m_keywordToken.raw);
        str.append("\nExpression: ");
        str.append(m_expression->toString());
        return str;
    }

    std::shared_ptr<StatementEvaluator> ArrowlessStatement::getEvaluator() const
    {
        return nullptr;
    }
}