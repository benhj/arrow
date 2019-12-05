/// (c) Ben Jones 2019

#include "FunctionExpressionEvaluator.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include "evaluator/IdentifierEvaluator.hpp"
#include "expressions/IdentifierExpression.hpp"
#include "parser/Parser.hpp"
#include "parser/LanguageException.hpp"
#include "statements/FunctionStatement.hpp"
#include "utility/ThreadManager.hpp"
#include <utility>

namespace arrow {
    FunctionExpressionEvaluator::FunctionExpressionEvaluator(FunctionExpression expression)
      : m_expression(std::move(expression))
    {
    }
    Type FunctionExpressionEvaluator::evaluate(Cache & cache) const
    {
        // Pull out the name of the function
        auto const name = m_expression.getName().raw;

        // Handle any built-in function here
        if(name == "join_all") {
            ThreadManager::joinAll();
            return {TypeDescriptor::Nil, false};
        }

        auto const callLineNumber = m_expression.getName().lineNumber;

        // Pull out the arguments being passed into the function
        auto const expressionColl = m_expression.getExpressionCollection();
        auto const t = expressionColl->getEvaluator()->evaluate(cache);
        auto const expressionCollEval = std::get<std::vector<Type>>(t.m_variantType);

        // Get the function to evaluate
        auto functionStatement = Parser::getFunction(name);
        if(!functionStatement) {
            throw LanguageException("Can't find function", callLineNumber);
        }

        // Get the parameters of the function signature
        auto const paramColl = functionStatement->getExpressionCollection();
        auto const p = paramColl->getEvaluator()->evaluate(cache);
        auto const paramCollEval = std::get<std::vector<Type>>(p.m_variantType);

        // Throw if signature mismatch
        if(paramCollEval.size() != expressionCollEval.size()) {
            throw LanguageException("Parameter indexing mismatch", callLineNumber);
        }

        // The function has its own completely
        // isolated, local cache, so need to create
        // a new one here.
        Cache newCache;

        // Push in parameters into new cache. The function
        // will then access these parameters
        auto const functionLineNumber = functionStatement->getName().lineNumber;
        auto param = std::begin(paramCollEval);
        for (auto const & expr : expressionCollEval) {
            auto const raw = std::get<std::string>(param->m_variantType);
            newCache.add({Lexeme::GENERIC_STRING, raw, functionLineNumber}, expr);
            ++param;
        }

        functionStatement->getEvaluator()->evaluate(newCache);
        auto const funcReturnIdentifier = functionStatement->getReturnIdentifier();
        if(funcReturnIdentifier.lexeme != Lexeme::NIL) {
            return newCache.get(funcReturnIdentifier);
        }
        return {TypeDescriptor::Nil, false};
    }
}