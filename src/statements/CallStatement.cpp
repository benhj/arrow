#include "CallStatement.hpp"
#include <utility>

namespace jasl {

    CallStatement::CallStatement() : Statement() {}
    CallStatement & CallStatement::withToken(Token token)
    {
        m_keywordToken = std::move(token);
        return *this;
    }
    CallStatement & CallStatement::withFunctionNameToken(Token token)
    {
        m_functionNameToken = std::move(token);
        return *this;
    }
    CallStatement & 
    CallStatement::withExpressionCollection(std::shared_ptr<Expression> expression)
    {
        m_expressionCollection = std::move(expression);
        return *this;
    }
    CallStatement & CallStatement::withIdentifier(Token identifier)
    {
        m_identifier = std::move(identifier);
        return *this;
    }

    std::string CallStatement::toString() const
    {
        std::string str("\nKeyword: ");
        str.append(m_keywordToken.raw);
        str.append("\nFunction name: ");
        str.append(m_functionNameToken.raw);
        str.append(m_expressionCollection->toString());
        str.append("\nIdentifier: ");
        str.append(m_identifier.raw);
        return str;
    }
}