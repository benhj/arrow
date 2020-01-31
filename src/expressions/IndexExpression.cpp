/// (c) Ben Jones 2019 - present

#include "IndexExpression.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

namespace arrow {

    namespace {

        template <typename T>
        Type getElement(Type statementType, std::shared_ptr<Expression> exp, Cache & cache)
        {
            auto container = std::get<std::vector<T>>(statementType.m_variantType);
            auto index = exp->getEvaluator()->evaluate(cache);

            if(index.m_descriptor != TypeDescriptor::Int) {
                throw LanguageException("Expected an integer index", exp->getLineNumber());
            }

            auto deduced = std::get<int64_t>(index.m_variantType);
            if(deduced >= static_cast<int64_t>(container.size())) {
                throw LanguageException("Index too large", exp->getLineNumber());
            }
            if constexpr(std::is_same_v<T, Type>) {
                return container[deduced];
            } else if constexpr(std::is_same_v<T, int64_t>) {
                return {TypeDescriptor::Int, container[deduced]};
            } else if constexpr(std::is_same_v<T, long double>) {
                return {TypeDescriptor::Real, container[deduced]};
            } else if constexpr(std::is_same_v<T, bool>) {
                return {TypeDescriptor::Bool, container[deduced]};
            } else if constexpr(std::is_same_v<T, std::string>) {
                return {TypeDescriptor::String, container[deduced]};
            } else if constexpr(std::is_same_v<T, char>) {
                return {TypeDescriptor::Byte, container[deduced]};
            }
        }

        Type getElementFromString(Type statementType, std::shared_ptr<Expression> exp, Cache & cache)
        {
            auto container = std::get<std::string>(statementType.m_variantType);
            auto index = exp->getEvaluator()->evaluate(cache);
            auto deduced = std::get<int64_t>(index.m_variantType);
            if(deduced >= static_cast<int64_t>(container.size())) {
                throw LanguageException("Index too large", exp->getLineNumber());
            }
            return {TypeDescriptor::Byte, container[deduced]};
        }
    }

    IndexExpression::IndexExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> 
    IndexExpression::getEvaluator() const
    {
        struct IndexExpressionEvaluator : public ExpressionEvaluator {
            IndexExpressionEvaluator(Token tok, std::shared_ptr<Expression> expression) 
              : m_tok(std::move(tok))
              , m_expression(std::move(expression))
            {
            }

            Type evaluate(Cache & cache) const override
            {
                if(!cache.has(m_tok)) {
                    throw LanguageException("Value for identifier not found", m_expression->getLineNumber());
                }

                auto type = cache.get(m_tok);

                // Deduce for map access
                if(type.m_descriptor == TypeDescriptor::Map) {
                    auto key = m_expression->getEvaluator()->evaluate(cache);
                    if(key.m_descriptor != TypeDescriptor::String &&
                       key.m_descriptor != TypeDescriptor::ListWord) {
                        throw LanguageException("Expected a string key", m_expression->getLineNumber());
                    }
                    auto container = std::get<std::map<std::string, Type>>(type.m_variantType);
                    auto keyStr = std::get<std::string>(key.m_variantType);
                    auto found = container.find(keyStr);
                    if(found != std::end(container)) {
                        return {found->second.m_descriptor, found->second.m_variantType};
                    }
                    throw LanguageException("Entry with key not found", m_expression->getLineNumber());
                }

                // Else deduce for array (vector) access
                if(type.m_descriptor != TypeDescriptor::List &&
                    type.m_descriptor != TypeDescriptor::ExpressionCollection &&
                    type.m_descriptor != TypeDescriptor::Ints &&
                    type.m_descriptor != TypeDescriptor::Reals &&
                    type.m_descriptor != TypeDescriptor::Bools &&
                    type.m_descriptor != TypeDescriptor::Strings &&
                    type.m_descriptor != TypeDescriptor::Bytes &&
                    type.m_descriptor != TypeDescriptor::String &&
                    type.m_descriptor != TypeDescriptor::ListWord) {
                    throw LanguageException("Incompatiable type for index", m_expression->getLineNumber());
                }

                switch (type.m_descriptor) {
                    case TypeDescriptor::Ints:
                        return getElement<int64_t>(std::move(type), std::move(m_expression), cache);
                    case TypeDescriptor::Reals:
                        return getElement<long double>(std::move(type), std::move(m_expression), cache);
                    case TypeDescriptor::Bools:
                        return getElement<bool>(std::move(type), std::move(m_expression), cache);
                    case TypeDescriptor::Strings:
                        return getElement<std::string>(std::move(type), std::move(m_expression), cache);
                    case TypeDescriptor::Bytes:
                        return getElement<char>(std::move(type), std::move(m_expression), cache);
                    case TypeDescriptor::String:
                        return getElementFromString(std::move(type), std::move(m_expression), cache);
                    case TypeDescriptor::ListWord:
                        return getElementFromString(std::move(type), std::move(m_expression), cache);
                    default: break;
                }
                return getElement<Type>(std::move(type), std::move(m_expression), cache);
            }
          private:
            Token m_tok;
            std::shared_ptr<Expression> m_expression;
        };
        return std::make_shared<IndexExpressionEvaluator>(m_identifier, m_indexExpression);
    }

    IndexExpression & IndexExpression::withIdentifierToken(Token identifier)
    {
        m_identifier = std::move(identifier);
        return *this;
    }

    IndexExpression & 
    IndexExpression::withIndexExpression(std::shared_ptr<Expression> indexExp)
    {
        m_indexExpression = std::move(indexExp);
        return *this;
    }

    Token IndexExpression::getIdentifierToken() const
    {
        return  m_identifier;
    }

    std::shared_ptr<Expression> IndexExpression::getIndexExpression() const
    {
        return m_indexExpression;
    }

    std::string IndexExpression::toString() const
    {
        /*
        std::string str("\nIndex expression: ");
        return str.append(m_identifier.raw);
        */
        return "";
    }
}