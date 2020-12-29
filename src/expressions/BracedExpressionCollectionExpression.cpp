/// (c) Ben Jones 2019

#include "BracedExpressionCollectionExpression.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>
#include <iostream>

namespace arrow {

    namespace {

        template <typename T>
        Type add(Environment & environment,
                 std::vector<std::shared_ptr<Expression>> expressions,
                 TypeDescriptor const arrayType) {

            if constexpr(std::is_same_v<T, std::vector<int64_t>> ||
                         std::is_same_v<T, std::vector<real>> ||
                         std::is_same_v<T, std::vector<bool>> ||
                         std::is_same_v<T, std::vector<std::string>> ||
                         std::is_same_v<T, std::vector<char>> ||
                         std::is_same_v<T, std::vector<PodType>>) {
                std::vector<Type> vecDeduced;
                for(auto const & expression : expressions) {
                    vecDeduced.emplace_back(expression->getEvaluator()->evaluate(environment));
                }
                return {arrayType, vecDeduced};
            } else {
                std::vector<T> vecDeduced;
                auto remType = TypeDescriptor::Nil;
                for(auto const & expression : expressions) {
                    auto evaluated = expression->getEvaluator()->evaluate(environment);
                    if(remType == TypeDescriptor::Nil) {
                        remType = evaluated.m_descriptor;
                    } 
                    // Incompatible types. All types in a {a, b, c} expression
                    // should be the same
                    else if(evaluated.m_descriptor != remType) {
                        throw LanguageException("Type mismatch in brace expression", expression->getLineNumber());
                    }
                    auto val = std::get<T>(evaluated.m_variantType);
                    vecDeduced.emplace_back(val);
                }
                return {arrayType, vecDeduced};
            }
        }
    }

    BracedExpressionCollectionExpression::BracedExpressionCollectionExpression(long const lineNumber) 
      : Expression(lineNumber)
    {
    }

    std::shared_ptr<ExpressionEvaluator> BracedExpressionCollectionExpression::getEvaluator() const
    {
        struct BracedExpressionCollectionExpressionEvaluator : 
               public ExpressionEvaluator
        {
            BracedExpressionCollectionExpressionEvaluator(BracedExpressionCollectionExpression ece)
              : m_ece(std::move(ece))
            {
            }
            Type evaluate(Environment & environment) const override
            {
                auto expressions = m_ece.getExpressionCollection();
                if(expressions.empty()) {
                    throw LanguageException("Empty array initialization", m_ece.getLineNumber());
                }

                std::vector<Type> bigEval;

                auto expression = std::begin(expressions);
                auto remType = (*expression)->getEvaluator()->evaluate(environment);

                if(remType.m_descriptor == TypeDescriptor::Int) {
                    return add<int64_t>(environment, std::move(expressions), TypeDescriptor::Ints);
                } else if(remType.m_descriptor == TypeDescriptor::Real) {
                    return add<real>(environment, std::move(expressions), TypeDescriptor::Reals);
                } else if(remType.m_descriptor == TypeDescriptor::Bool) {
                    return add<bool>(environment, std::move(expressions), TypeDescriptor::Bools);
                } else if(remType.m_descriptor == TypeDescriptor::String) {
                    return add<std::string>(environment, std::move(expressions), TypeDescriptor::Strings);
                } else if(remType.m_descriptor == TypeDescriptor::Byte) {
                    return add<char>(environment, std::move(expressions), TypeDescriptor::Bytes);
                } else if(remType.m_descriptor == TypeDescriptor::Pod) {
                    return add<PodType>(environment, std::move(expressions), TypeDescriptor::Pods);
                } else if(remType.m_descriptor == TypeDescriptor::Ints) {
                    return add<std::vector<int64_t>>(environment, std::move(expressions), TypeDescriptor::Arrays);
                } else if(remType.m_descriptor == TypeDescriptor::Reals) {
                    return add<std::vector<real>>(environment, std::move(expressions), TypeDescriptor::Arrays);
                } else if(remType.m_descriptor == TypeDescriptor::Bools) {
                    return add<std::vector<bool>>(environment, std::move(expressions), TypeDescriptor::Arrays);
                } else if(remType.m_descriptor == TypeDescriptor::Strings) {
                    return add<std::vector<std::string>>(environment, std::move(expressions), TypeDescriptor::Arrays);
                } else if(remType.m_descriptor == TypeDescriptor::Bytes) {
                    return add<std::vector<char>>(environment, std::move(expressions), TypeDescriptor::Arrays);
                } else if(remType.m_descriptor == TypeDescriptor::Pods) {
                    return add<std::vector<PodType>>(environment, std::move(expressions), TypeDescriptor::Arrays);
                } else {
                    throw LanguageException("Type mismatch in brace expression", (*expression)->getLineNumber());
                }

            }
          private:
            BracedExpressionCollectionExpression m_ece;
        };
        return std::make_shared<BracedExpressionCollectionExpressionEvaluator>(*this);
    }

    void BracedExpressionCollectionExpression::addExpression(std::shared_ptr<Expression> expression)
    {
        m_expressions.emplace_back(std::move(expression));
    }

    std::vector<std::shared_ptr<Expression>> const &
    BracedExpressionCollectionExpression::getExpressionCollection() const
    {
        return m_expressions;
    }

    std::string BracedExpressionCollectionExpression::toString() const
    {
        if(!m_expressions.empty()) {
            std::string str("\nBegin braced expression collection: ");
            for(auto const & exp : m_expressions) {
                str.append(exp->toString());
            }
            str.append("\nEnd braced expression collection.");
            return str;
        }
        return "";
    }
}