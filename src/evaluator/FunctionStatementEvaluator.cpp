/// (c) Ben Jones 2019

#include "FunctionStatementEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

namespace arrow {
    FunctionStatementEvaluator::FunctionStatementEvaluator(FunctionStatement statement)
      : m_statement(std::move(statement))
    {
    }
    void FunctionStatementEvaluator::evaluate(Cache & cache) const
    {
        auto const name = m_statement.getName().raw;
        auto const statements = m_statement.getBodyStatements();
        for(auto const & statement : statements) {
            statement->getEvaluator()->evaluate(cache);
        }
        auto const returnIdentifier = m_statement.getReturnIdentifier();
        if(returnIdentifier.lexeme != Lexeme::NIL && !cache.has(returnIdentifier)) {
            throw LanguageException("Can't find return value in function statement",
                                    returnIdentifier.lineNumber);
        }
    }
}