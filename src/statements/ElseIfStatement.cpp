#include "ElseIfStatement.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include <utility>

namespace jasl {

    ElseIfStatement::ElseIfStatement() : Statement() {}
    ElseIfStatement & ElseIfStatement::withToken(Token token)
    {
        m_keywordToken = std::move(token);
        return *this;
    }
    ElseIfStatement & ElseIfStatement::withExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
        return *this;
    }
    void ElseIfStatement::addBodyStatement(std::shared_ptr<Statement> bodyStatement)
    {
        m_bodyStatements.emplace_back(std::move(bodyStatement));
    }

    std::string ElseIfStatement::toString() const
    {
        std::string str("\nKeyword: ");
        str.append(m_keywordToken.raw);
        str.append("\nExpression: ");
        str.append(m_expression->toString());
        str.append("\nBegin body statements:\n");
        for(auto const & statement : m_bodyStatements) {
            str.append(statement->toString());
        }
        str.append("\nEnd body statements.");
        return str;
    }

    std::shared_ptr<StatementEvaluator> ElseIfStatement::getEvaluator() const
    {
        return nullptr;
    }
}