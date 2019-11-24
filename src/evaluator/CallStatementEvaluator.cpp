#include "CallStatementEvaluator.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include "evaluator/IdentifierEvaluator.hpp"
#include "expressions/IdentifierExpression.hpp"
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

        // Get the function to evaluate
        auto functionStatement = Parser::getFunction(name);
        if(!functionStatement) {
            std::string error("Can't find function on line ");
            error.append(std::to_string(m_statement.getName().lineNumber));
            throw std::runtime_error(error);
        }

        // Get the parameters of the function signature
        auto const paramColl = functionStatement->getExpressionCollection();
        auto const p = paramColl->getEvaluator()->evaluate(cache);
        auto const paramCollEval = std::get<std::vector<Type>>(p.m_variantType);

        // Throw if signature mismatch
        if(paramCollEval.size() != expressionCollEval.size()) {
            auto lineCall = m_statement.getName().lineNumber;
            auto lineFunc = functionStatement->getName().lineNumber;
            std::string error("Parameter indexing mismatch on lines ");
            error.append(std::to_string(lineCall)).append(" and ");
            error.append(std::to_string(lineFunc));
            throw std::runtime_error(error);
        }

        // The function has its own completely
        // isolated, local cache, so need to create
        // a new one here.
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
        auto const funcReturnIdentifier = functionStatement->getReturnIdentifier();
        auto const result = newCache.get(funcReturnIdentifier.raw);
        auto const callReturnIdentifier = m_statement.getReturnIdentifier();
        cache.add(callReturnIdentifier.raw, result);
    }
}