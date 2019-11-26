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
        auto const callLineNumber = m_statement.getName().lineNumber;

        // Pull out the arguments being passed into the function
        auto const expressionColl = m_statement.getExpressionCollection();
        auto const t = expressionColl->getEvaluator()->evaluate(cache);
        auto const expressionCollEval = std::get<std::vector<Type>>(t.m_variantType);

        // Get the function to evaluate
        auto functionStatement = Parser::getFunction(name);
        auto const functionLineNumber = functionStatement->getName().lineNumber;
        if(!functionStatement) {
            std::string error("Can't find function on line ");
            error.append(std::to_string(callLineNumber));
            throw std::runtime_error(error);
        }

        // Get the parameters of the function signature
        auto const paramColl = functionStatement->getExpressionCollection();
        auto const p = paramColl->getEvaluator()->evaluate(cache);
        auto const paramCollEval = std::get<std::vector<Type>>(p.m_variantType);

        // Throw if signature mismatch
        if(paramCollEval.size() != expressionCollEval.size()) {
            auto lineCall = m_statement.getName().lineNumber;
            std::string error("Parameter indexing mismatch on lines ");
            error.append(std::to_string(lineCall)).append(" and ");
            error.append(std::to_string(functionLineNumber));
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
            newCache.add({Lexeme::GENERIC_STRING, raw, functionLineNumber}, expr);
            ++param;
        }

        functionStatement->getEvaluator()->evaluate(newCache);
        auto const funcReturnIdentifier = functionStatement->getReturnIdentifier();
        if(funcReturnIdentifier.lexeme != Lexeme::NIL) {
            auto const result = newCache.get(funcReturnIdentifier);
            auto const callReturnIdentifier = m_statement.getReturnIdentifier();
            if(callReturnIdentifier.lexeme == Lexeme::NIL) {
                std::string error("Return mismatch on line numbers ");
                error.append(std::to_string(callLineNumber)).append(" and ");
                error.append(std::to_string(functionLineNumber));
                throw std::runtime_error(error);
            }
            cache.add(callReturnIdentifier, result);
        }
    }
}