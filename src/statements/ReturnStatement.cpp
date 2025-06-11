/// (c) Ben Jones 2019 - present

#include "ReturnStatement.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include <utility>

namespace arrow {

    ReturnStatement::ReturnStatement(long const lineNumber)
    : Statement(lineNumber)
    {

    }
    std::string ReturnStatement::toString() const
    {
        return ""; // todo
    }

    std::shared_ptr<StatementEvaluator> ReturnStatement::getEvaluator() const
    {
        struct ReturnStatementEvaluator final : public StatementEvaluator
        {
            StatementResult evaluate(Environment &) const override
            {
                return StatementResult::Return;
            }
        };
        return std::make_shared<ReturnStatementEvaluator>();
    }
}