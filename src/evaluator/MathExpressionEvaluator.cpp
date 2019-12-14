/// (c) Ben Jones 2019

#include "MathExpressionEvaluator.hpp"
#include "expressions/MathExpression.hpp"
#include "parser/LanguageException.hpp"
#include "representation/TypeDescriptor.hpp"
#include <utility>
#include <cmath>

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
            throw LanguageException("Incompatible types on line", m_lineNumber);
        }
        long double operator()(std::vector<Type> left,
                               std::vector<Type> right) const
        {
            throw LanguageException("Incompatible types on line", m_lineNumber);
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
                if constexpr(!std::is_same_v<Left, char> &&
                             std::is_same_v<Left, Right>) {
                    if(m_op == "%") {
                        return std::fmod(left, right);
                    } 
                } else if constexpr(!std::is_same_v<Left, long double> &&
                             !std::is_same_v<Left, char> &&
                             std::is_same_v<Left, Right>) {
                    if(m_op == "^") {
                        return left ^ right;
                    }
                } else if constexpr(!std::is_same_v<Left, long double> &&
                                     std::is_same_v<Left, Right>) {
                    if(m_op == "^") {
                        return left ^ right;
                    }
                }
            }
            throw LanguageException("Incompatible types on line", m_lineNumber);
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

        // String concatenation
        if(deducedLeft.m_descriptor == TypeDescriptor::String) {
            if(op.raw == "+") {
                auto str = std::get<std::string>(deducedLeft.m_variantType);
                if(deducedRight.m_descriptor == TypeDescriptor::String ||
                   deducedRight.m_descriptor == TypeDescriptor::ListWord) {
                    str = str.append(std::get<std::string>(deducedRight.m_variantType));
                    return {TypeDescriptor::String, str};
                } else if(deducedRight.m_descriptor == TypeDescriptor::Byte) {
                    str.push_back(std::get<char>(deducedRight.m_variantType));
                    return {TypeDescriptor::String, str};
                } else if(deducedRight.m_descriptor == TypeDescriptor::Int) {
                    str = str.append(std::to_string(std::get<int64_t>(deducedRight.m_variantType)));
                    return {TypeDescriptor::String, str};
                } else if(deducedRight.m_descriptor == TypeDescriptor::Real) {
                    str = str.append(std::to_string(std::get<long double>(deducedRight.m_variantType)));
                    return {TypeDescriptor::String, str};
                } else if(deducedRight.m_descriptor == TypeDescriptor::Bool) {
                    str = str.append(std::to_string(std::get<bool>(deducedRight.m_variantType)));
                    return {TypeDescriptor::String, str};
                }
            }
        }

        auto intType{true};
        if(deducedLeft.m_descriptor == TypeDescriptor::Real ||
           deducedRight.m_descriptor == TypeDescriptor::Real) {
            intType = false;
        }
        if(!intType) {
            if(deducedLeft.m_descriptor == TypeDescriptor::Int) {
                auto const val = std::get<int64_t>(deducedLeft.m_variantType);
                deducedLeft.m_descriptor = TypeDescriptor::Real;
                deducedLeft.m_variantType = static_cast<long double>(val);
            }
            if(deducedRight.m_descriptor == TypeDescriptor::Int) {
                auto const val = std::get<int64_t>(deducedRight.m_variantType);
                deducedRight.m_descriptor = TypeDescriptor::Real;
                deducedRight.m_variantType = static_cast<long double>(val);
            }
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