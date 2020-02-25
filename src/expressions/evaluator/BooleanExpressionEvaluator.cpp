/// (c) Ben Jones 2019 - present

#include "BooleanExpressionEvaluator.hpp"
#include "expressions/BooleanExpression.hpp"
#include "parser/LanguageException.hpp"
#include "representation/TypeDescriptor.hpp"
#include <utility>

namespace arrow {

    struct BooleanEvaluator {
        BooleanEvaluator(std::string op, long const lineNumber)
        : m_op(std::move(op))
        , m_lineNumber(lineNumber)
        {
        }

        bool operator()(std::string left,
                        std::string right) const
        {
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
                throw LanguageException("Bad operator for type String", m_lineNumber);
            }
        }
        bool operator()(std::vector<Type> left,
                        std::vector<Type> right) const
        {
            if(m_op == "=") {
                return left == right;
            } else if(m_op == "/=") {
                return left != right;
            } 
            else {
                throw LanguageException("Bad operator for type List", m_lineNumber);
            }
        }

        bool operator()(PodType left,
                        PodType right) const
        {
            if(m_op == "=") {
                return left == right;
            }else if(m_op == "/=") {
                return left != right;
            } 
            throw LanguageException("Bad operator for type pod", m_lineNumber);
        }
        bool operator()(std::vector<PodType> left,
                        std::vector<PodType> right) const
        {
            throw LanguageException("Bad operator for type pod", m_lineNumber);
        }

        bool operator()(std::map<std::string, Type> left,
                        std::map<std::string, Type> right) const
        {
            throw LanguageException("Bad operator for type Map", m_lineNumber);
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
                                 !std::is_same_v<Left, std::vector<real>> &&
                                 !std::is_same_v<Left, std::vector<char>> &&
                                 !std::is_same_v<Left, std::vector<std::string>>) {
                        return left && right;
                    }
                } else if(m_op == "||") {
                    if constexpr(!std::is_same_v<Left, std::vector<bool>> &&
                                 !std::is_same_v<Left, std::vector<int64_t>>  &&
                                 !std::is_same_v<Left, std::vector<real>> &&
                                 !std::is_same_v<Left, std::vector<char>> &&
                                 !std::is_same_v<Left, std::vector<std::string>>) {
                        return left || right;
                    }
                } else if(m_op == "/=") {
                    return left != right;
                } else {
                    throw LanguageException("Bad operator for type", m_lineNumber);
                }
            }
            throw LanguageException("Incompatible types", m_lineNumber);
        }
      private:
        std::string m_op;
        long m_lineNumber;
    };

    BooleanExpressionEvaluator::BooleanExpressionEvaluator(BooleanExpression expression)
      : m_expression(std::move(expression))
    {
    }

    Type BooleanExpressionEvaluator::evaluate(Environment & environment) const
    {
        auto leftEval = m_expression.getExpressionLeft()->getEvaluator();
        auto rightEval = m_expression.getExpressionRight()->getEvaluator();
        auto op = m_expression.getOperator();
        auto deducedLeft = leftEval->evaluate(environment);
        auto deducedRight = rightEval->evaluate(environment);
        BooleanEvaluator evaluator{op.raw, m_expression.getLineNumber()};
        auto res = std::visit(evaluator, 
                  deducedLeft.m_variantType,
                  deducedRight.m_variantType);
        return {TypeDescriptor::Bool, res};
    }
}