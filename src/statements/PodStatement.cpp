/// (c) Ben Jones 2020 - present

#include "PodStatement.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include "evaluator/PodStatementEvaluator.hpp"
#include <utility>

namespace arrow {

    PodStatement::PodStatement(long const lineNumber)
    : Statement(lineNumber)
    {
    }
    PodStatement & PodStatement::withToken(Token token)
    {
        m_keywordToken = std::move(token);
        return *this;
    }
    PodStatement & PodStatement::withNameToken(Token token)
    {
        m_nameToken = std::move(token);
        return *this;
    }
    PodStatement & 
    PodStatement::withExpressionCollection(std::shared_ptr<Expression> expression)
    {
        m_expressionCollection = std::move(expression);
        return *this;
    }

    Token const & PodStatement::getName() const
    {
        return m_nameToken;
    }
    std::shared_ptr<Expression> const & PodStatement::getExpressionCollection() const
    {
        return m_expressionCollection;
    }

    std::string PodStatement::toString() const
    {
        return ""; // TODO
    }

    std::shared_ptr<StatementEvaluator> PodStatement::getEvaluator() const
    {
        return std::make_shared<PodStatementEvaluator>(*this);
    }
}