#include "BracedExpressionCollectionExpression.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include <utility>
#include <iostream>

namespace arrow {

    namespace {

        template <typename T>
        Type add(Cache & cache,
                 std::vector<std::shared_ptr<Expression>> expressions,
                 TypeDescriptor const arrayType) {

            std::vector<T> vecDeduced(expressions.size());
            auto remType = TypeDescriptor::Nil;
            for(auto const & expression : expressions) {

                auto evaluated = expression->getEvaluator()->evaluate(cache);
                if(remType == TypeDescriptor::Nil) {
                    remType = evaluated.m_descriptor;
                } 
                // Incompatible types. All types in a {a, b, c} expression
                // should be the same
                else if(evaluated.m_descriptor != remType) {
                    std::string error("Type mismatch on line... (TODO)");
                    throw std::runtime_error(error);
                }
                auto val = std::get<T>(evaluated.m_variantType);
                vecDeduced.emplace_back(val);
            }

            return {arrayType, vecDeduced};
        }
    }

    BracedExpressionCollectionExpression::BracedExpressionCollectionExpression()
     : Expression()
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
            Type evaluate(Cache & cache) const override
            {
                auto expressions = m_ece.getExpressionCollection();
                std::vector<Type> bigEval;

                auto expression = std::begin(expressions);
                auto remType = (*expression)->getEvaluator()->evaluate(cache);

                if(remType.m_descriptor == TypeDescriptor::Int) {
                    return add<int64_t>(cache, std::move(expressions), TypeDescriptor::Ints);
                } else if(remType.m_descriptor == TypeDescriptor::Real) {
                    return add<long double>(cache, std::move(expressions), TypeDescriptor::Reals);
                } else if(remType.m_descriptor == TypeDescriptor::Bool) {
                    return add<bool>(cache, std::move(expressions), TypeDescriptor::Bools);
                } else if(remType.m_descriptor == TypeDescriptor::String) {
                    return add<std::string>(cache, std::move(expressions), TypeDescriptor::Strings);
                } else if(remType.m_descriptor == TypeDescriptor::Byte) {
                    return add<char>(cache, std::move(expressions), TypeDescriptor::Bytes);
                }

            }
          private:
            BracedExpressionCollectionExpression m_ece;
        };
        return std::make_shared<BracedExpressionCollectionExpressionEvaluator>(*this);
    }

    DecayType BracedExpressionCollectionExpression::decayType() const
    {
        return DecayType::DECAYS_TO_NONE;
    }

    void BracedExpressionCollectionExpression::addExpression(std::shared_ptr<Expression> expression)
    {
        m_expressions.emplace_back(std::move(expression));
    }

    std::vector<std::shared_ptr<Expression>> 
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