/// (c) Ben Jones 2019 - present

#include "LoopBreakStatement.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include <utility>

namespace arrow {

    LoopBreakStatement::LoopBreakStatement(long const lineNumber)
    : Statement(lineNumber)
    {

    }
    std::string LoopBreakStatement::toString() const
    {
        return ""; // todo
    }

    std::shared_ptr<StatementEvaluator> LoopBreakStatement::getEvaluator() const
    {
        struct LoopBreakStatementEvaluator : public StatementEvaluator
        {
            StatementResult evaluate(Environment &) const override
            {
                return StatementResult::Break;
            }
        };
        return std::make_shared<LoopBreakStatementEvaluator>();
    }
}