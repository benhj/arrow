/// (c) Ben Jones 2019 - present

#include "StartStatement.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include <utility>

namespace arrow {
    StartStatement::StartStatement(long const lineNumber) 
      : Statement(lineNumber)
    {
    }
    StartStatement & StartStatement::withToken(Token token)
    {
        m_keywordToken = std::move(token);
        return *this;
    }
    void StartStatement::addBodyStatement(std::shared_ptr<Statement> bodyStatement)
    {
        m_bodyStatements.emplace_back(std::move(bodyStatement));
    }

    std::vector<std::shared_ptr<Statement>>
    StartStatement::getBodyStatements() const
    {
        return m_bodyStatements;
    }

    std::string StartStatement::toString() const
    {
        std::string str("\nKeyword: ");
        str.append(m_keywordToken.raw);
        str.append("\nBegin body statements:\n");
        for(auto const & statement : m_bodyStatements) {
            str.append(statement->toString());
        }
        str.append("\nEnd body statements.");
        return str;
    }

    std::shared_ptr<StatementEvaluator> StartStatement::getEvaluator() const
    {

        struct StartStatementEvaluator : public StatementEvaluator
        {
            explicit StartStatementEvaluator(StartStatement statement)
              : m_statement(std::move(statement))
            {
            }

            void evaluate(Cache & cache) const override
            {
                auto const bodyStatements = m_statement.getBodyStatements();
                for(auto const & inner : bodyStatements) {
                    inner->getEvaluator()->evaluate(cache);
                }
            }
          private:
            StartStatement m_statement;
        };

        return std::make_shared<StartStatementEvaluator>(*this);
    }
}