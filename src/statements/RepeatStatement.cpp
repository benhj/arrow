#include "RepeatStatement.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include "evaluator/RepeatStatementEvaluator.hpp"
#include <utility>

namespace arrow {

    RepeatStatement::RepeatStatement() : Statement() {}
    RepeatStatement & RepeatStatement::withToken(Token token)
    {
        m_keywordToken = std::move(token);
        return *this;
    }
    RepeatStatement & RepeatStatement::withExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
        return *this;
    }
    void RepeatStatement::addBodyStatement(std::shared_ptr<Statement> bodyStatement)
    {
        m_bodyStatements.emplace_back(std::move(bodyStatement));
    }

    std::string RepeatStatement::toString() const
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
    std::shared_ptr<Expression> RepeatStatement::getExpression() const
    {
        return m_expression;
    }
    std::vector<std::shared_ptr<Statement>>
    RepeatStatement::getBodyStatements() const
    {
        return m_bodyStatements;
    }

    std::shared_ptr<StatementEvaluator> RepeatStatement::getEvaluator() const
    {
        return std::make_shared<RepeatStatementEvaluator>(*this);
    }
}