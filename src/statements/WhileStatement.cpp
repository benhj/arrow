#include "WhileStatement.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include "evaluator/WhileStatementEvaluator.hpp"
#include <utility>

namespace arrow {

    WhileStatement::WhileStatement() : Statement() {}
    WhileStatement & WhileStatement::withToken(Token token)
    {
        m_keywordToken = std::move(token);
        return *this;
    }
    WhileStatement & WhileStatement::withExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
        return *this;
    }
    void WhileStatement::addBodyStatement(std::shared_ptr<Statement> bodyStatement)
    {
        m_bodyStatements.emplace_back(std::move(bodyStatement));
    }
    std::vector<std::shared_ptr<Statement>> 
    WhileStatement::getBodyStatements() const
    {
        return m_bodyStatements;
    }

    std::string WhileStatement::toString() const
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
    std::shared_ptr<Expression> WhileStatement::getExpression() const
    {
        return m_expression;
    }
    std::shared_ptr<StatementEvaluator> WhileStatement::getEvaluator() const
    {
        return std::make_shared<WhileStatementEvaluator>(*this);
    }
}