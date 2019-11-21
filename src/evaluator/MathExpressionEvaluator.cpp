#include "MathExpressionEvaluator.hpp"
#include "expressions/MathExpression.hpp"
#include "representation/TypeDescriptor.hpp"
#include <utility>

namespace jasl {

    struct MathEvaluator {
        MathEvaluator(std::string op)
        : m_op(std::move(op))
        {
        }

        long double operator()(std::string left,
                               std::string right) const
        {
            throw std::runtime_error("Bad types for Math operation.");
        }
        long double operator()(std::vector<std::string> left,
                               std::vector<std::string> right) const
        {
            throw std::runtime_error("Bad types for Math operation.");
        }

        template <class Left, class Right>
        long double operator()(Left left, Right right) const
        {
            if constexpr(!std::is_same_v<Left, std::string> &&
                         !std::is_same_v<Right, std::string> &&
                         !std::is_same_v<Left, std::vector<std::string>> &&
                         !std::is_same_v<Right, std::vector<std::string>>) {
                if(m_op == "+") {
                    return left + right;
                } else if(m_op == "-") {
                    return left - right;
                } else if(m_op == "*") {
                    return left * right;
                } else if(m_op == "/") {
                    return left / right;
                }
                if constexpr(!std::is_same_v<Left, long double> &&
                             std::is_same_v<Left, Right>) {
                    if(m_op == "%") {
                        return left % right;
                    } else if(m_op == "^") {
                        return left ^ right;
                    }
                }
                throw std::runtime_error("Bad operator for type.");
            }
            throw std::runtime_error("Incompatible types.");
        }
      private:
        std::string m_op;
    };

    MathExpressionEvaluator::MathExpressionEvaluator(MathExpression expression)
      : m_expression(std::move(expression))
    {
    }

    Type MathExpressionEvaluator::evaluate() const
    {
        auto leftEval = m_expression.getExpressionLeft()->getEvaluator();
        auto rightEval = m_expression.getExpressionRight()->getEvaluator();
        auto op = m_expression.getOperator();
        auto deducedLeft = leftEval->evaluate();
        auto deducedRight = rightEval->evaluate();

        MathEvaluator evaluator{op.raw};
        auto res = std::visit(evaluator, 
                  deducedLeft.m_variantType,
                  deducedRight.m_variantType);
        return {TypeDescriptor::Real, res};
    }
}