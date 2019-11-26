#include "ForStatementEvaluator.hpp"
#include "ExpressionEvaluator.hpp"
#include "expressions/IdentifierExpression.hpp"
#include "statements/ElseIfStatement.hpp"
#include <utility>

namespace arrow {

    namespace {
        void evaluateBody(std::vector<std::shared_ptr<Statement>> bodyStatements,
                          Cache & cache)
        {
            for(auto const & statement : bodyStatements) {
                statement->getEvaluator()->evaluate(cache); 
            }
        }

        template <typename T>
        void evaluateContainerElements(T elements,
                                       std::vector<std::shared_ptr<Statement>> bodyStatements,
                                       Token indexer,
                                       Cache & cache)
        {
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
                }
                evaluateBody(bodyStatements, cache);
                cache.popCacheLayer();
            }
        }
    }

    ForStatementEvaluator::ForStatementEvaluator(ForStatement statement)
      : m_statement(std::move(statement))
    {
    }

    void ForStatementEvaluator::evaluate(Cache & cache) const
    {
        auto indexer = m_statement.getIndexer();
        auto identifier = m_statement.getIdentifier();
        IdentifierExpression exp;
        exp.withIdentifierToken(identifier);
        auto evaled = exp.getEvaluator()->evaluate(cache);
        if(evaled.m_descriptor != TypeDescriptor::List &&
           evaled.m_descriptor != TypeDescriptor::ExpressionCollection &&
           evaled.m_descriptor != TypeDescriptor::Ints &&
           evaled.m_descriptor != TypeDescriptor::Reals &&
           evaled.m_descriptor != TypeDescriptor::Bools &&
           evaled.m_descriptor != TypeDescriptor::Strings) {
            throw std::runtime_error("Bad type descriptor in for statement expression.");
        }

        auto bodyStatements = m_statement.getBodyStatements();
        if(evaled.m_descriptor == TypeDescriptor::List ||
           evaled.m_descriptor == TypeDescriptor::ExpressionCollection) {
            auto elements = std::get<std::vector<Type>>(evaled.m_variantType);
            evaluateContainerElements(elements, std::move(bodyStatements), std::move(indexer), cache);
        } else if(evaled.m_descriptor == TypeDescriptor::Ints) {
            auto elements = std::get<std::vector<int64_t>>(evaled.m_variantType);
            evaluateContainerElements(elements, std::move(bodyStatements), std::move(indexer), cache);
        } else if(evaled.m_descriptor == TypeDescriptor::Reals) {
            auto elements = std::get<std::vector<long double>>(evaled.m_variantType);
            evaluateContainerElements(elements, std::move(bodyStatements), std::move(indexer), cache);
        } else if(evaled.m_descriptor == TypeDescriptor::Bools) {
            auto elements = std::get<std::vector<bool>>(evaled.m_variantType);
            evaluateContainerElements(elements, std::move(bodyStatements), std::move(indexer), cache);
        } else if(evaled.m_descriptor == TypeDescriptor::Strings) {
            auto elements = std::get<std::vector<std::string>>(evaled.m_variantType);
            evaluateContainerElements(elements, std::move(bodyStatements), std::move(indexer), cache);
        }
    }
}