#include "FunctionStatement.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include <utility>

namespace jasl {

    FunctionStatement::FunctionStatement() : Statement() {}
    FunctionStatement & FunctionStatement::withToken(Token token)
    {
        m_keywordToken = std::move(token);
        return *this;
    }
    FunctionStatement & FunctionStatement::withTypeToken(Token token)
    {
        m_typeToken = std::move(token);
        return *this;
    }
    FunctionStatement & FunctionStatement::withNameToken(Token token)
    {
        m_nameToken = std::move(token);
        return *this;
    }
    FunctionStatement & FunctionStatement::withReturnIdentifierToken(Token token)
    {
        m_returnIdentifier = std::move(token);
        return *this;
    }
    FunctionStatement & 
    FunctionStatement::withExpressionCollection(std::shared_ptr<Expression> expression)
    {
        m_expressionCollection = std::move(expression);
        return *this;
    }
    void FunctionStatement::addBodyStatement(std::shared_ptr<Statement> bodyStatement)
    {
        m_bodyStatements.emplace_back(std::move(bodyStatement));
    }

    std::string FunctionStatement::toString() const
    {
        std::string str("\nKeyword: ");
        str.append(m_keywordToken.raw);
        str.append("\nReturn type: ");
        str.append(m_typeToken.raw);
        str.append("\nFunction name: ");
        str.append(m_nameToken.raw);
        str.append(m_expressionCollection->toString());
        str.append("\nReturn identifier: ");
        str.append(m_returnIdentifier.raw);
        str.append("\nBegin body statements:\n");
        for(auto const & statement : m_bodyStatements) {
            str.append(statement->toString());
        }
        str.append("\nEnd body statements.");
        return str;
    }

    std::shared_ptr<StatementEvaluator> FunctionStatement::getEvaluator() const
    {
        return nullptr;
    }
}