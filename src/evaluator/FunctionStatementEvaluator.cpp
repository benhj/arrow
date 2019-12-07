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
        auto const statements = m_statement.getBodyStatements();
        auto const returnIdentifier = m_statement.getReturnIdentifier();
        for(auto const & statement : statements) {
            auto const result = statement->getEvaluator()->evaluate(cache);
            if(result == StatementResult::Return) {
                break;
            }
        }
        if(returnIdentifier.lexeme != Lexeme::NIL && !cache.has(returnIdentifier)) {
            throw LanguageException("Can't find return value in function statement",
                                    returnIdentifier.lineNumber);
        }
        return StatementResult::Continue;
    }
}