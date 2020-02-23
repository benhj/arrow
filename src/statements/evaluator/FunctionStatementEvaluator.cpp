/// (c) Ben Jones 2019

#include "FunctionStatementEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

namespace arrow {
    FunctionStatementEvaluator::FunctionStatementEvaluator(FunctionStatement statement)
      : m_statement(std::move(statement))
    {
    }
    StatementResult FunctionStatementEvaluator::evaluate(Environment & environment) const
    {
        auto const name = m_statement.getName().raw;
        auto const innerStatement = m_statement.getInnerStatement();
        auto const returnIdentifier = m_statement.getReturnIdentifier();
        auto res = innerStatement->getEvaluator()->evaluate(environment);
        if(returnIdentifier.lexeme != Lexeme::NIL && !environment.has(returnIdentifier.raw)) {
            throw LanguageException("Can't find return value in function statement",
                                    returnIdentifier.lineNumber);
        }
        if(res == StatementResult::Exit) {
            return res;
        }
        return StatementResult::Continue;
    }
}