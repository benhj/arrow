#include "BooleanExpressionEvaluator.hpp"
#include "expressions/BooleanExpression.hpp"
#include "representation/TypeDescriptor.hpp"
#include <utility>

namespace jasl {

    struct BooleanEvaluator {
        BooleanEvaluator(std::string op)
        : m_op(std::move(op))
        {
        }

        bool operator()(std::string left,
                        std::string right) const
        {
            auto res{false};
            if(m_op == "=") {
                return left == right;
            } else if(m_op == "<") {
                return left < right;
            } else if(m_op == "<=") {
                return left <= right;
            } else if(m_op == ">") {
                return left > right;
            } else if(m_op == ">=") {
                return left >= right;
            } else if(m_op == "/=") {
                return left != right;
            } else {
                throw std::runtime_error("Bad operator for type List.");
            }
        }
        bool operator()(std::vector<std::string> left,
                        std::vector<std::string> right) const
        {
            auto res{false};
            if(m_op == "=") {
                return left == right;
            } else if(m_op == "<") {
                return left < right;
            } else if(m_op == "<=") {
                return left <= right;
            } else if(m_op == ">") {
                return left > right;
            } else if(m_op == ">=") {
                return left >= right;
            } else if(m_op == "/=") {
                return left != right;
            } else {
                throw std::runtime_error("Bad operator for type List.");
            }
        }

        template <class Left, class Right>
        bool operator()(Left left, Right right) const
        {
            if constexpr(std::is_same_v<Left, Right>) {
                if(m_op == "=") {
                    return left == right;
                } else if(m_op == "<") {
                    return left < right;
                } else if(m_op == "<=") {
                    return left <= right;
                } else if(m_op == ">") {
                    return left > right;
                } else if(m_op == ">=") {
                    return left >= right;
                } else if(m_op == "&&") {
                    return left && right;
                } else if(m_op == "||") {
                    return left || right;
                } else if(m_op == "/=") {
                    return left != right;
                } else {
                    throw std::runtime_error("Bad operator for type.");
                }
            }
            throw std::runtime_error("Incompatible types.");
        }
      private:
        std::string m_op;
    };

    BooleanExpressionEvaluator::BooleanExpressionEvaluator(BooleanExpression expression)
      : m_expression(std::move(expression))
    {
    }

    Type BooleanExpressionEvaluator::evaluate() const
    {
        auto leftEval = m_expression.getExpressionLeft()->getEvaluator();
        auto rightEval = m_expression.getExpressionRight()->getEvaluator();
        auto op = m_expression.getOperator();
        auto deducedLeft = leftEval->evaluate();
        auto deducedRight = rightEval->evaluate();

        BooleanEvaluator evaluator{op.raw};
        auto res = std::visit(evaluator, 
                  deducedLeft.m_variantType,
                  deducedRight.m_variantType);
        return {TypeDescriptor::Bool, res};
    }
}