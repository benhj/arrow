#include "ArrowStatement.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include <utility>

namespace jasl {
    
    ArrowStatement::ArrowStatement() : Statement() {}
    ArrowStatement & ArrowStatement::withToken(Token token)
    {
        m_keywordToken = std::move(token);
        return *this;
    }
    ArrowStatement & ArrowStatement::withExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
        return *this;
    }
    ArrowStatement & ArrowStatement::withIdentifier(Token identifier)
    {
        m_identifier = std::move(identifier);
        return *this;
    }

    std::string ArrowStatement::toString() const
    {
        std::string str("\nKeyword: ");
        str.append(m_keywordToken.raw);
        str.append("\nExpression: ");
        str.append(m_expression->toString());
        str.append("\nIdentifier: ");
        str.append(m_identifier.raw);
        return str;
    }

    std::shared_ptr<StatementEvaluator> ArrowStatement::getEvaluator() const
    {
        return nullptr;
    }
}