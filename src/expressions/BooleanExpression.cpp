#include "BooleanExpression.hpp"

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

    BooleanExpression::BooleanExpression() 
    : Expression()
    , m_operatorExp(std::make_shared<OperatorExpression>())
    {
    }

    Type BooleanExpression::evaluate() const
    {
        auto leftEval = getExpressionLeft()->evaluate();
        auto rightEval = getExpressionRight()->evaluate();
        auto op = getOperator();
        BooleanEvaluator evaluator{op.raw};
        auto res = std::visit(evaluator, 
                              leftEval.m_variantType,
                              rightEval.m_variantType);
        return {TypeDescriptor::Bool, res};
    }

    DecayType BooleanExpression::decayType() const
    {
        return DecayType::DECAYS_TO_BOOL;
    }

    BooleanExpression & 
    BooleanExpression::withLeft(std::shared_ptr<Expression> expressionLeft)
    {
        m_operatorExp->withLeft(std::move(expressionLeft));
        return *this;
    }
    BooleanExpression & 
    BooleanExpression::withRight(std::shared_ptr<Expression> expressionRight)
    {
        m_operatorExp->withRight(std::move(expressionRight));
        return *this;
    }
    BooleanExpression & BooleanExpression::withOperator(Token op)
    {
        m_operatorExp->withOperator(std::move(op));
        return *this;
    }
    std::shared_ptr<Expression> BooleanExpression::getExpressionLeft() const
    {
        return m_operatorExp->getExpressionLeft();
    }
    std::shared_ptr<Expression> BooleanExpression::getExpressionRight() const
    {
        return m_operatorExp->getExpressionRight();
    }
    Token BooleanExpression::getOperator() const
    {
        return m_operatorExp->getOperator();
    }
    std::string BooleanExpression::toString() const
    {
        return m_operatorExp->toString();
    }

}