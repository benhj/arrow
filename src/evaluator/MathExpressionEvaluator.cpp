#include "MathExpressionEvaluator.hpp"
#include "expressions/MathExpression.hpp"
#include "representation/TypeDescriptor.hpp"
#include <utility>

namespace arrow {

    struct MathEvaluator {
        MathEvaluator(std::string op, long const lineNumber)
        : m_op(std::move(op))
        , m_lineNumber(lineNumber)
        {
        }

        long double operator()(std::string left,
                               std::string right) const
        {
            std::string error("Incompatible types on line ");
            error.append(std::to_string(m_lineNumber));
            throw std::runtime_error(error);
        }
        long double operator()(std::vector<Type> left,
                               std::vector<Type> right) const
        {
            std::string error("Incompatible types on line ");
            error.append(std::to_string(m_lineNumber));
            throw std::runtime_error(error);
        }

        template <class Left, class Right>
        long double operator()(Left left, Right right) const
        {
            if constexpr(std::is_same_v<Left, Right> &&
                         !std::is_same_v<Left, std::string> &&
                         !std::is_same_v<Left, std::vector<Type>> &&
                         !std::is_same_v<Left, std::vector<bool>> &&
                         !std::is_same_v<Left, std::vector<int64_t>>  &&
                         !std::is_same_v<Left, std::vector<long double>> &&
                         !std::is_same_v<Left, std::vector<std::string>> &&
                         !std::is_same_v<Left, std::vector<char>> &&
                         !std::is_same_v<Left, char>) {
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
                             !std::is_same_v<Left, char> &&
                             std::is_same_v<Left, Right>) {
                    if(m_op == "%") {
                        return left % right;
                    } else if(m_op == "^") {
                        return left ^ right;
                    }
                } else if constexpr(!std::is_same_v<Left, long double> &&
                                     std::is_same_v<Left, Right>) {
                    if(m_op == "^") {
                        return left ^ right;
                    }
                }
            }
            std::string error("Incompatible types on line ");
            error.append(std::to_string(m_lineNumber));
            throw std::runtime_error(error);
        }
      private:
        std::string m_op;
        long m_lineNumber;
    };

    MathExpressionEvaluator::MathExpressionEvaluator(MathExpression expression)
      : m_expression(std::move(expression))
    {
    }

    Type MathExpressionEvaluator::evaluate(Cache & cache) const
    {
        auto leftEval = m_expression.getExpressionLeft()->getEvaluator();
        auto rightEval = m_expression.getExpressionRight()->getEvaluator();
        auto op = m_expression.getOperator();
        auto deducedLeft = leftEval->evaluate(cache);
        auto deducedRight = rightEval->evaluate(cache);

        auto intType{true};
        if(deducedLeft.m_descriptor == TypeDescriptor::Real ||
           deducedRight.m_descriptor == TypeDescriptor::Real) {
            intType = false;
        }

        MathEvaluator evaluator{op.raw, op.lineNumber};
        auto res = std::visit(evaluator, 
                  deducedLeft.m_variantType,
                  deducedRight.m_variantType);

        if(!intType) {
            return {TypeDescriptor::Real, res};
        } else {
            return {TypeDescriptor::Int, static_cast<int64_t>(res)};
        }

    }
}