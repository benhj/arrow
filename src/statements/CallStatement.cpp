#include "CallStatement.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include "evaluator/CallStatementEvaluator.hpp"
#include <utility>

namespace arrow {

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

    Token CallStatement::getName() const
    {
        return m_functionNameToken;
    }
    std::shared_ptr<Expression> CallStatement::getExpressionCollection() const
    {
        return m_expressionCollection;
    }
    Token CallStatement::getReturnIdentifier() const
    {
        return m_identifier;
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

    std::shared_ptr<StatementEvaluator> CallStatement::getEvaluator() const
    {
        return std::make_shared<CallStatementEvaluator>(*this);
    }
}