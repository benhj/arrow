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
                                  std::vector<Token> indices,
                                  Cache & cache)
        {

            auto evaluated = StatementResult::Continue;

            auto it = std::begin(elements);
            for(; it != std::end(elements); ++it) {
                if(it >= std::end(elements)) {
                    break;
                }
                cache.pushCacheLayer();
                if constexpr (std::is_same_v<typename T::value_type, Type>) {
                    auto step = 0;
                    for(auto const & index : indices) {
                        if((it + step) == std::end(elements)) {
                            break;
                        }
                        cache.add(index, *(it + step));
                        ++step;
                    }
                } else if constexpr (std::is_same_v<typename T::value_type, int64_t>) {
                    auto step = 0;
                    for(auto const & index : indices) {
                        if((it + step) == std::end(elements)) {
                            break;
                        }
                        cache.add(index, {TypeDescriptor::Int, *(it + step)});
                        ++step;
                    }
                } else if constexpr (std::is_same_v<typename T::value_type, long double>) {
                    auto step = 0;
                    for(auto const & index : indices) {
                        if((it + step) == std::end(elements)) {
                            break;
                        }
                        cache.add(index, {TypeDescriptor::Real, *(it + step)});
                        ++step;
                    }
                } else if constexpr (std::is_same_v<typename T::value_type, bool>) {
                    auto step = 0;
                    for(auto const & index : indices) {
                        if((it + step) == std::end(elements)) {
                            break;
                        }
                        cache.add(index, {TypeDescriptor::Bool, *(it + step)});
                        ++step;
                    }
                } else if constexpr (std::is_same_v<typename T::value_type, std::string>) {
                    auto step = 0;
                    for(auto const & index : indices) {
                        if((it + step) == std::end(elements)) {
                            break;
                        }
                        cache.add(index, {TypeDescriptor::String, *(it + step)});
                        ++step;
                    }
                } else if constexpr (std::is_same_v<typename T::value_type, char>) {
                    auto step = 0;
                    for(auto const & index : indices) {
                        if((it + step) == std::end(elements)) {
                            break;
                        }
                        cache.add(index, {TypeDescriptor::Byte, *(it + step)});
                        ++step;
                    }
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
        auto indices = m_statement.getIndices();
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
            return evaluateContainerElements(elements, std::move(innerStatement), std::move(indices), cache);
        } else if(evaled.m_descriptor == TypeDescriptor::Ints) {
            auto elements = std::get<std::vector<int64_t>>(evaled.m_variantType);
            return evaluateContainerElements(elements, std::move(innerStatement), std::move(indices), cache);
        } else if(evaled.m_descriptor == TypeDescriptor::Reals) {
            auto elements = std::get<std::vector<long double>>(evaled.m_variantType);
            return evaluateContainerElements(elements, std::move(innerStatement), std::move(indices), cache);
        } else if(evaled.m_descriptor == TypeDescriptor::Bools) {
            auto elements = std::get<std::vector<bool>>(evaled.m_variantType);
            return evaluateContainerElements(elements, std::move(innerStatement), std::move(indices), cache);
        } else if(evaled.m_descriptor == TypeDescriptor::Strings) {
            auto elements = std::get<std::vector<std::string>>(evaled.m_variantType);
            return evaluateContainerElements(elements, std::move(innerStatement), std::move(indices), cache);
        } else if(evaled.m_descriptor == TypeDescriptor::String || evaled.m_descriptor == TypeDescriptor::ListWord) {
            auto elements = std::get<std::string>(evaled.m_variantType);
            return evaluateContainerElements(elements, std::move(innerStatement), std::move(indices), cache);
        } else if(evaled.m_descriptor == TypeDescriptor::Byte) {
            auto elements = std::get<std::vector<char>>(evaled.m_variantType);
            return evaluateContainerElements(elements, std::move(innerStatement), std::move(indices), cache);
        }
        return StatementResult::Continue;
    }
}