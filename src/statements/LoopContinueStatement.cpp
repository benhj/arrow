/// (c) Ben Jones 2025 - present

#include "LoopContinueStatement.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include <utility>

namespace arrow {

    LoopContinueStatement::LoopContinueStatement(long const lineNumber)
    : Statement(lineNumber)
    {

    }
    std::string LoopContinueStatement::toString() const
    {
        return ""; // todo
    }

    std::shared_ptr<StatementEvaluator> LoopContinueStatement::getEvaluator() const
    {
        struct LoopContinueStatementEvaluator : public StatementEvaluator
        {
            StatementResult evaluate(Environment &) const override
            {
                return StatementResult::LoopContinue;
            }
        };
        return std::make_shared<LoopContinueStatementEvaluator>();
    }
}