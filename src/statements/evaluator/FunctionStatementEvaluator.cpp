/// (c) Ben Jones 2019

#include "FunctionStatementEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

namespace arrow {
    FunctionStatementEvaluator::FunctionStatementEvaluator(FunctionStatement statement)
      : m_statement(std::move(statement))
    {
    }
    StatementResult FunctionStatementEvaluator::evaluate(Cache & cache) const
    {
        auto const name = m_statement.getName().raw;
        auto const innerStatement = m_statement.getInnerStatement();
        auto const returnIdentifier = m_statement.getReturnIdentifier();
        (void)innerStatement->getEvaluator()->evaluate(cache);
        if(returnIdentifier.lexeme != Lexeme::NIL && !cache.has(returnIdentifier)) {
            throw LanguageException("Can't find return value in function statement",
                                    returnIdentifier.lineNumber);
        }
        return StatementResult::Continue;
    }
}