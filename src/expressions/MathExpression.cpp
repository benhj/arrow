#include "MathExpression.hpp"

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

    MathExpression::MathExpression() 
    : Expression()
    , m_operatorExp(std::make_shared<OperatorExpression>())
    {
    }

    std::shared_ptr<Evaluator> MathExpression::getEvaluator() const
    {
        return nullptr;
    }
/*
    Type MathExpression::evaluate() const
    {
        auto leftEval = getExpressionLeft()->evaluate();
        auto rightEval = getExpressionRight()->evaluate();
        auto op = getOperator();
        MathEvaluator evaluator{op.raw};
        auto res = std::visit(evaluator, 
                              leftEval.m_variantType,
                              rightEval.m_variantType);
        return {TypeDescriptor::Real, res};
    }
*/
    DecayType MathExpression::decayType() const
    {
        return DecayType::DECAYS_TO_REAL;
    }

    MathExpression & MathExpression::withLeft(std::shared_ptr<Expression> expressionLeft)
    {
        m_operatorExp->withLeft(std::move(expressionLeft));
        return *this;
    }
    MathExpression & MathExpression::withRight(std::shared_ptr<Expression> expressionRight)
    {
        m_operatorExp->withRight(std::move(expressionRight));
        return *this;
    }
    MathExpression & MathExpression::withOperator(Token op)
    {
        m_operatorExp->withOperator(std::move(op));
        return *this;
    }
    std::shared_ptr<Expression> MathExpression::getExpressionLeft() const
    {
        return m_operatorExp->getExpressionLeft();
    }
    std::shared_ptr<Expression> MathExpression::getExpressionRight() const
    {
        return m_operatorExp->getExpressionRight();
    }
    Token MathExpression::getOperator() const
    {
        return m_operatorExp->getOperator();
    }
    std::string MathExpression::toString() const
    {
        return m_operatorExp->toString();
    }
}