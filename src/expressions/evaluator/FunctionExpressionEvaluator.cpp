/// (c) Ben Jones 2019

#include "FunctionExpressionEvaluator.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "statements/evaluator/StatementEvaluator.hpp"
#include "IdentifierEvaluator.hpp"
#include "expressions/IdentifierExpression.hpp"
#include "parser/LanguageException.hpp"
#include "statements/FunctionStatement.hpp"
#include "utility/ThreadManager.hpp"
#include <utility>

namespace arrow {
    FunctionExpressionEvaluator::FunctionExpressionEvaluator(FunctionExpression expression)
      : m_expression(std::move(expression))
    {
    }
    Type FunctionExpressionEvaluator::evaluate(Environment & environment) const
    {
        // Pull out the name of the function
        auto const name = m_expression.getName().raw;

        // Handle any built-in function here
        if(name == "join_all") {
            ThreadManager::joinAll();
            return {TypeDescriptor::Nothing, false};
        }

        auto const callLineNumber = m_expression.getName().lineNumber;

        // Pull out the arguments being passed into the function
        auto const expressionColl = m_expression.getExpressionCollection();
        auto const t = expressionColl->getEvaluator()->evaluate(environment);
        auto const & expressionCollEval = std::get<std::vector<Type>>(t.m_variantType);

        // Get the function to evaluate; if can't find function,
        // try instead to get a pod.
        auto functionStatement = environment.getFunction(name);
        std::shared_ptr<PodStatement> podStatement;

        std::shared_ptr<Expression> paramColl;
        if(functionStatement) {
            paramColl = functionStatement->getExpressionCollection();
        } else {
            podStatement = environment.getPod(name);
            if(!podStatement) {
                throw LanguageException("Can't find function or pod", callLineNumber);
            }
            paramColl = podStatement->getExpressionCollection();
        }

        // Get the parameters of the function/pod signature
        auto const p = paramColl->getEvaluator()->evaluate(environment);
        auto const & paramCollEval = std::get<std::vector<Type>>(p.m_variantType);

        // Throw if signature mismatch
        if(paramCollEval.size() != expressionCollEval.size()) {
            throw LanguageException("Parameter indexing mismatch", callLineNumber);
        }

        // The function has its own completely
        // isolated, local environment but the function and pod
        // lookup tables and argument map should be inherited.
        Environment newEnvironment(environment.getOutputStream());
        newEnvironment.withFunctions(environment.getFunctions());
        newEnvironment.withPods(environment.getPods());
        newEnvironment.withProgramArgs(environment.getProgramArgs());

        PodType podType;

        // Push in parameters into new environment. The function
        // will then access these parameters
        auto param = std::begin(paramCollEval);
        for (auto const & expr : expressionCollEval) {
            auto const & raw = std::get<std::string>(param->m_variantType);

            if(functionStatement) {
                newEnvironment.add(raw, expr);
            } 
            // Must be a pod statement
            else {
                podType.add(raw, expr);
            }
            ++param;
        }

        if(podStatement) {
            return {TypeDescriptor::Pod, podType};
        }

        auto result = functionStatement->getEvaluator()->evaluate(newEnvironment);
        if(result == StatementResult::Exit) {
            environment.setExitState();
        }
        auto const funcReturnIdentifier = functionStatement->getReturnIdentifier();
        if(funcReturnIdentifier.lexeme != Lexeme::NIL) {
            return newEnvironment.get(funcReturnIdentifier.raw);
        }

        return {TypeDescriptor::Nothing, false};
    }
}