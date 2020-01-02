/// (c) Ben Jones 2019

#include "ForStatementEvaluator.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "expressions/IdentifierExpression.hpp"
#include "parser/LanguageException.hpp"
#include "statements/ElseIfStatement.hpp"
#include <utility>

namespace arrow {

    namespace {

        template <typename T>
        StatementResult
        evaluateContainerElements(T elements,
                                  std::shared_ptr<Statement> innerStatement,
                                  Token indexer,
                                  Cache & cache)
        {

            auto evaluated = StatementResult::Continue;
            for (auto const & element : elements) {
                cache.pushCacheLayer();
                if constexpr (std::is_same_v<typename T::value_type, Type>) {
                    cache.add(indexer, element);
                } else if constexpr (std::is_same_v<typename T::value_type, int64_t>) {
                    cache.add(indexer, {TypeDescriptor::Int, element});
                } else if constexpr (std::is_same_v<typename T::value_type, long double>) {
                    cache.add(indexer, {TypeDescriptor::Real, element});
                } else if constexpr (std::is_same_v<typename T::value_type, bool>) {
                    cache.add(indexer, {TypeDescriptor::Bool, element});
                } else if constexpr (std::is_same_v<typename T::value_type, std::string>) {
                    cache.add(indexer, {TypeDescriptor::String, element});
                } else if constexpr (std::is_same_v<typename T::value_type, char>) {
                    cache.add(indexer, {TypeDescriptor::Byte, element});
                }
                evaluated = innerStatement->getEvaluator()->evaluate(cache);
                cache.popCacheLayer();
                if(evaluated != StatementResult::Continue) {
                    return evaluated;
                }
            }
            return StatementResult::Continue;
        }
    }

    ForStatementEvaluator::ForStatementEvaluator(ForStatement statement)
      : m_statement(std::move(statement))
    {
    }

    StatementResult ForStatementEvaluator::evaluate(Cache & cache) const
    {
        auto indexer = m_statement.getIndexer();
        auto identifier = m_statement.getIdentifier();
        IdentifierExpression exp(m_statement.getLineNumber());
        exp.withIdentifierToken(identifier);
        auto evaled = exp.getEvaluator()->evaluate(cache);
        if(evaled.m_descriptor != TypeDescriptor::List &&
           evaled.m_descriptor != TypeDescriptor::ExpressionCollection &&
           evaled.m_descriptor != TypeDescriptor::Ints &&
           evaled.m_descriptor != TypeDescriptor::Reals &&
           evaled.m_descriptor != TypeDescriptor::Bools &&
           evaled.m_descriptor != TypeDescriptor::Strings &&
           evaled.m_descriptor != TypeDescriptor::String &&
           evaled.m_descriptor != TypeDescriptor::ListWord &&
           evaled.m_descriptor != TypeDescriptor::Bytes) {
            throw LanguageException("Bad type descriptor in for statement expression",
                                    identifier.lineNumber);
        }

        auto innerStatement = m_statement.getInnerStatement();
        if(evaled.m_descriptor == TypeDescriptor::List ||
           evaled.m_descriptor == TypeDescriptor::ExpressionCollection) {
            auto elements = std::get<std::vector<Type>>(evaled.m_variantType);
            return evaluateContainerElements(elements, std::move(innerStatement), std::move(indexer), cache);
        } else if(evaled.m_descriptor == TypeDescriptor::Ints) {
            auto elements = std::get<std::vector<int64_t>>(evaled.m_variantType);
            return evaluateContainerElements(elements, std::move(innerStatement), std::move(indexer), cache);
        } else if(evaled.m_descriptor == TypeDescriptor::Reals) {
            auto elements = std::get<std::vector<long double>>(evaled.m_variantType);
            return evaluateContainerElements(elements, std::move(innerStatement), std::move(indexer), cache);
        } else if(evaled.m_descriptor == TypeDescriptor::Bools) {
            auto elements = std::get<std::vector<bool>>(evaled.m_variantType);
            return evaluateContainerElements(elements, std::move(innerStatement), std::move(indexer), cache);
        } else if(evaled.m_descriptor == TypeDescriptor::Strings) {
            auto elements = std::get<std::vector<std::string>>(evaled.m_variantType);
            return evaluateContainerElements(elements, std::move(innerStatement), std::move(indexer), cache);
        } else if(evaled.m_descriptor == TypeDescriptor::String || evaled.m_descriptor == TypeDescriptor::ListWord) {
            auto elements = std::get<std::string>(evaled.m_variantType);
            return evaluateContainerElements(elements, std::move(innerStatement), std::move(indexer), cache);
        } else if(evaled.m_descriptor == TypeDescriptor::Byte) {
            auto elements = std::get<std::vector<char>>(evaled.m_variantType);
            return evaluateContainerElements(elements, std::move(innerStatement), std::move(indexer), cache);
        }
        return StatementResult::Continue;
    }
}