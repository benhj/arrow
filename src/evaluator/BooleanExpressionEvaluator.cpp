#include "BooleanExpressionEvaluator.hpp"
#include "expressions/BooleanExpression.hpp"
#include "representation/TypeDescriptor.hpp"
#include <utility>

namespace arrow {

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
        bool operator()(std::vector<Type> left,
                        std::vector<Type> right) const
        {
            // auto res{false};
            // if(m_op == "=") {
            //     return left == right;
            // } 
            // else if(m_op == "<") {
            //     return left < right;
            // } else if(m_op == "<=") {
            //     return left <= right;
            // } else if(m_op == ">") {
            //     return left > right;
            // } else if(m_op == ">=") {
            //     return left >= right;
            // } 
            // else if(m_op == "/=") {
            //     return left != right;
            // } 
            // else {
                throw std::runtime_error("Bad operator for type List.");
            // }
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
                    if constexpr(!std::is_same_v<Left, std::vector<bool>> &&
                                 !std::is_same_v<Left, std::vector<int64_t>>  &&
                                 !std::is_same_v<Left, std::vector<long double>> &&
                                 !std::is_same_v<Left, std::vector<char>> &&
                                 !std::is_same_v<Left, std::vector<std::string>>) {
                        return left && right;
                    }
                } else if(m_op == "||") {
                    if constexpr(!std::is_same_v<Left, std::vector<bool>> &&
                                 !std::is_same_v<Left, std::vector<int64_t>>  &&
                                 !std::is_same_v<Left, std::vector<long double>> &&
                                 !std::is_same_v<Left, std::vector<char>> &&
                                 !std::is_same_v<Left, std::vector<std::string>>) {
                        return left || right;
                    }
                } else if(m_op == "/=") {
                    return left != right;
                } else {
                    throw std::runtime_error("Bad operator for type.");
                }
            }
            std::string error("Incompatible types on line ");
            throw std::runtime_error(error);
        }
      private:
        std::string m_op;
    };

    BooleanExpressionEvaluator::BooleanExpressionEvaluator(BooleanExpression expression)
      : m_expression(std::move(expression))
    {
    }

    Type BooleanExpressionEvaluator::evaluate(Cache & cache) const
    {
        auto leftEval = m_expression.getExpressionLeft()->getEvaluator();
        auto rightEval = m_expression.getExpressionRight()->getEvaluator();
        auto op = m_expression.getOperator();
        auto deducedLeft = leftEval->evaluate(cache);
        auto deducedRight = rightEval->evaluate(cache);
        BooleanEvaluator evaluator{op.raw};
        auto res = std::visit(evaluator, 
                  deducedLeft.m_variantType,
                  deducedRight.m_variantType);
        return {TypeDescriptor::Bool, res};
    }
}