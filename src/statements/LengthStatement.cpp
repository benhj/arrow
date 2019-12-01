#include "LengthStatement.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

namespace arrow {
    
    namespace {
        template <typename T>
        void set(Type type, Token identifier, Cache & cache) {
            auto casted = std::get<std::vector<T>>(type.m_variantType);
            cache.add(identifier, {TypeDescriptor::Int, static_cast<int64_t>(casted.size())});
        }
    }

    LengthStatement::LengthStatement(long const lineNumber, ArrowStatement arrowStatement) 
      : Statement(lineNumber)
      , m_arrowStatement(std::move(arrowStatement))
    {
    }

    std::string LengthStatement::toString() const
    {
        return m_arrowStatement.toString();
    }

    std::shared_ptr<StatementEvaluator> LengthStatement::getEvaluator() const
    {
        
        struct LengthStatementEvaluator : public StatementEvaluator
        {
            LengthStatementEvaluator(LengthStatement statement)
              : m_statement(std::move(statement))
            {
            }
            void evaluate(Cache & cache) const override
            {
                auto const expression = m_statement.getExpression();
                auto const type = expression->getEvaluator()->evaluate(cache);
                if(type.m_descriptor != TypeDescriptor::List &&
                   type.m_descriptor != TypeDescriptor::Ints &&
                   type.m_descriptor != TypeDescriptor::Reals &&
                   type.m_descriptor != TypeDescriptor::Strings &&
                   type.m_descriptor != TypeDescriptor::Bools &&
                   type.m_descriptor != TypeDescriptor::Bytes &&
                   type.m_descriptor != TypeDescriptor::String) {
                    throw LanguageException("Not a container", m_statement.getLineNumber());
                }

                auto identifier = m_statement.getIdentifier();
                switch (type.m_descriptor) {
                    case TypeDescriptor::List:
                        set<Type>(type, identifier, cache);
                        return;
                    case TypeDescriptor::Ints:
                        set<int64_t>(type, identifier, cache);
                        return;
                    case TypeDescriptor::Reals:
                        set<long double>(type, identifier, cache);
                        return;
                    case TypeDescriptor::Bools:
                        set<bool>(type, identifier, cache);
                        return;
                    case TypeDescriptor::Strings:
                        set<std::string>(type, identifier, cache);
                        return;
                    case TypeDescriptor::Bytes:
                        set<char>(type, identifier, cache);
                        return;
                    case TypeDescriptor::String: {
                        auto casted = std::get<std::string>(type.m_variantType);
                        cache.add(identifier, {TypeDescriptor::Int, static_cast<int64_t>(casted.size())});
                    }
                    default:
                        break;
                }
            }
          private:
            LengthStatement m_statement;
        };
        return std::make_shared<LengthStatementEvaluator>(*this);
    }

    LengthStatement const & LengthStatement::withToken(Token token) const
    {
        m_arrowStatement.withToken(std::move(token));
        return *this;
    }

    Token LengthStatement::getToken() const
    {
        return m_arrowStatement.getToken();
    }

    std::shared_ptr<Expression> LengthStatement::getExpression() const
    {
        return m_arrowStatement.getExpression();
    }

    Token LengthStatement::getIdentifier() const
    {
        return m_arrowStatement.getIdentifier();
    }
}