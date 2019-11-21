#include "ArrowlessStatement.hpp"
#include <utility>

namespace jasl {

    ArrowlessStatement::ArrowlessStatement() : Statement() {}
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

    std::string ArrowlessStatement::toString() const
    {
        std::string str("\nKeyword: ");
        str.append(m_keywordToken.raw);
        str.append("\nExpression: ");
        str.append(m_expression->toString());
        return str;
    }
}