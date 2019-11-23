#include "CallStatementEvaluator.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include "parser/Parser.hpp"
#include "statements/FunctionStatement.hpp"
#include <utility>

namespace arrow {
    CallStatementEvaluator::CallStatementEvaluator(CallStatement statement)
      : m_statement(std::move(statement))
    {
    }
    void CallStatementEvaluator::evaluate(Cache & cache) const
    {
        // Pull out the name of the function
        auto const name = m_statement.getName().raw;

        // Pull out the arguments being passed into the function
        auto const expressionColl = m_statement.getExpressionCollection();
        auto const t = expressionColl->getEvaluator()->evaluate(cache);
        auto const expressionCollEval = std::get<std::vector<Type>>(t.m_variantType);

        // Pull out the return identifier
        auto const returnIdentifier = m_statement.getReturnIdentifier();

        // Get the function to evaluate
        auto functionStatement = Parser::getFunction(name);

        // Get the parameters of the function signature
        auto const paramColl = functionStatement->getExpressionCollection();
        auto const p = paramColl->getEvaluator()->evaluate(cache);
        auto const paramCollEval = std::get<std::vector<Type>>(p.m_variantType);

        // Throw if signature mismatch
        if(paramCollEval.size() != expressionCollEval.size()) {
            throw std::runtime_error("Parameter indexing mismatch.");
        }

        Cache newCache;

        // Push in parameters into new cache. The function
        // will then access these parameters
        auto param = std::begin(paramCollEval);
        for (auto const & expr : expressionCollEval) {
            auto const raw = std::get<std::string>(param->m_variantType);
            newCache.add(raw, expr);
            ++param;
        }
        functionStatement->getEvaluator()->evaluate(newCache);
        auto poppedReturn = newCache.getAndPopReturnValue();
        cache.add(returnIdentifier.raw, poppedReturn);
    }
}