#include "PutStatementEvaluator.hpp"
#include "ExpressionEvaluator.hpp"
#include "PrimitiveStatementEvaluator.hpp"
#include <utility>

namespace arrow {
    PutStatementEvaluator::PutStatementEvaluator(PutStatement statement)
      : m_statement(std::move(statement))
    {
    }
    void PutStatementEvaluator::evaluate(Cache & cache) const
    {
        auto const identifier = m_statement.getIdentifier().raw;
        if(cache.has(identifier)) {
            auto t = cache.get(identifier);
            std::string newType;
            if(t.m_descriptor == TypeDescriptor::Int) { newType = "int"; }
            else if(t.m_descriptor == TypeDescriptor::Real) { newType = "real"; }
            else if(t.m_descriptor == TypeDescriptor::String) { newType = "string"; }
            else if(t.m_descriptor == TypeDescriptor::Bool) { newType = "bool"; }
            else if(t.m_descriptor == TypeDescriptor::List) { newType = "list"; }
            else if(t.m_descriptor == TypeDescriptor::Ints) { newType = "ints"; }
            else if(t.m_descriptor == TypeDescriptor::Reals) { newType = "reals"; }
            else if(t.m_descriptor == TypeDescriptor::Strings) { newType = "strings"; }
            else if(t.m_descriptor == TypeDescriptor::Bools) { newType = "bools"; }
            else {
                throw std::runtime_error(std::string("Couldn't evaluate ").append(identifier));
            }
            auto arrowStatement = m_statement.getArrowStatement();
            arrowStatement.withToken(Token(Lexeme::GENERIC_STRING, newType));
            PrimitiveStatementEvaluator(std::move(arrowStatement)).evaluate(cache);
        } else {
            throw std::runtime_error(std::string("Couldn't evaluate ").append(identifier));
        }
    }
}