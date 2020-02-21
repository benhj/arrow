/// (c) Ben Jones 2019

#include "MatchesExpressionEvaluator.hpp"
#include "ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>
#include <vector>

namespace arrow {

    namespace {

        bool listMatch(std::vector<Type> const & left,
                       std::vector<Type> const & right,
                       Environment & environment,
                       long const lineNumber);

        // To describe how list tokens match.
        enum class MatchType
        {
            Exact,     // [hello], [hello]
            QVar,      // [hello], [?var]
            QQVar,     // [hello], [??var]
            SingleEq,  // [hello], [=]
            DoubleEq,  // [hello], [==]
            NoMatch    // [hello], [goodbye]
        };

        MatchType matches(Type left, Type right, Environment & environment, long const lineNumber)
        {   
            if(left == right) {
                return MatchType::Exact;
            } else if (left.m_descriptor == TypeDescriptor::List &&
                   right.m_descriptor == TypeDescriptor::List) {
                auto newLeft = std::get<std::vector<Type>>(left.m_variantType);
                auto newRight = std::get<std::vector<Type>>(right.m_variantType);
                if(listMatch(newLeft, newRight, environment, lineNumber)) {
                    return MatchType::Exact;
                } else {
                    return MatchType::NoMatch;
                }
            }

            if(right.m_descriptor == TypeDescriptor::SingleEqual) {
                return MatchType::SingleEq;
            } else if(right.m_descriptor == TypeDescriptor::DoubleEqual) {
                return MatchType::DoubleEq;
            } else if(right.m_descriptor == TypeDescriptor::QString) {
                return MatchType::QVar;
            } else if(right.m_descriptor == TypeDescriptor::QQString) {
                return MatchType::QQVar;
            }
            return MatchType::NoMatch;
        }
        bool handleExact(std::vector<Type> const & left,
                         std::vector<Type> const & right,
                         std::vector<Type>::const_iterator & itLeft,
                         std::vector<Type>::const_iterator & itRight)
        {
            ++itLeft;
            ++itRight;

            // Boundary -- check if there are more tokens
            // in list 2 to be processed. If so, we have come
            // to the end of the first string and the overall match
            // has failed.
            if(itLeft != std::end(left) && itRight == std::end(right)) {
                return false;
            }

            return true;
        }
        bool handleEq(std::vector<Type> const & left,
                      std::vector<Type> const & right,
                      std::vector<Type>::const_iterator & itLeft,
                      std::vector<Type>::const_iterator & itRight)
        {
            ++itLeft;
            ++itRight;

            // Boundary -- check if there are more tokens
            // in list 2 to be processed. If so, we have come
            // to the end of the first string and the overall match
            // has failed.
            if(itLeft != std::end(left) && itRight == std::end(right)) {
                return false;
            }
            return true;
        }
        bool handleEqEq(std::vector<Type> const & left,
                        std::vector<Type> const & right,
                        std::vector<Type>::const_iterator & itLeft,
                        std::vector<Type>::const_iterator & itRight,
                        Environment & environment,
                        long const lineNumber)
        {
            ++itLeft;
            ++itRight;

            // If we are at the end of the second string, as in
            // [end of string ==] it means that even if there are
            // more tokens in the first string, we match them all
            // given '=='.
            
            if(itRight == std::end(right)) {
                return true;
            }

            // More token to process in first. We keep looping until we
            // find a match.
            while(matches(*itLeft, *itRight, environment, lineNumber) == MatchType::NoMatch) {
                ++itLeft;
                if(itLeft == std::end(left)) {
                    return false;
                }
            }
            ++itLeft;
            ++itRight;

            if(itLeft == std::end(left) &&
               itRight == std::end(right)) {
                return true;
            }

            return (itRight != std::end(right));
        }
        bool handleQVar(std::vector<Type> const & left,
                        std::vector<Type> const & right,
                        std::vector<Type>::const_iterator & itLeft,
                        std::vector<Type>::const_iterator & itRight,
                        Environment & environment,
                        long const lineNumber)
        {

            // The identifier is stored as a raw string, without the initial '?'
            auto const var = std::get<std::string>(itRight->m_variantType);
            auto const toStore = *itLeft;

            ++itLeft;
            ++itRight;

            // Boundary -- check if there are more tokens
            // in list 2 to be processed. If so, we have come
            // to the end of the first string and the overall match
            // has failed.
            if(itLeft != std::end(left) && itRight == std::end(right)) {
                return false;
            }


            environment.add(var, toStore);
            return true;
        }
        bool handleQQVar(std::vector<Type> const & left,
                         std::vector<Type> const & right,
                         std::vector<Type>::const_iterator & itLeft,
                         std::vector<Type>::const_iterator & itRight,
                         Environment & environment,
                         long const lineNumber)
        {

            // The identifier is stored as a raw string, without the initial '?'
            auto const var = std::get<std::string>(itRight->m_variantType);
            ++itRight;

            // Create list of elements to store ??dat inside of
            std::vector<Type> elements;
            elements.push_back(*itLeft);
            ++itLeft;

            // If we are at the end of the second string, as in
            // [end of string ??var] it means that even if there are
            // more tokens in the first string, we match them all
            // given '??var' so can simply store all tokens in the
            // list and return true.
            if(itRight == std::end(right)) {
                while(itLeft != std::end(left)) {
                    elements.push_back(*itLeft);
                    ++itLeft;
                }
                environment.add(var, {TypeDescriptor::List, std::move(elements)});
                return true;
            }

            // More tokens to process in left. We keep looping until we
            // find a match.
            auto matchType = matches(*itLeft, *itRight, environment, lineNumber);
            while(matchType == MatchType::NoMatch) {
                elements.push_back(*itLeft);
                ++itLeft;
                if(itLeft == std::end(left)) {
                    return false;
                }
                matchType = matches(*itLeft, *itRight, environment, lineNumber);
            }

            ++itLeft;
            ++itRight;
/*
            if (itRight == std::end(right)) {
                return false;
            }
*/
            environment.add(var, {TypeDescriptor::List, std::move(elements)});
            return true;
        }

        bool listMatch(std::vector<Type> const & left,
                       std::vector<Type> const & right,
                       Environment & environment,
                       long const lineNumber)
        {

            // Edge-case 1. Two empty string always match.
            if(left.empty() && right.empty()) {
                return true;
            }

            // Edge-case 2. If one string is empty, there
            // is no match.
            if(left.empty() || right.empty()) {
                return false;
            }

            // Edge-case 3. If the second string is longer, then
            // there is no match.
            if(right.size() > left.size()) {
                return false;
            }

            // Edge-case 4. If the second string is simply [==],
            // it matches the whole of the first string so we
            // can return early.
            if(right.size() == 1 && 
               right.at(0).m_descriptor == TypeDescriptor::DoubleEqual) {
                return true;
            }

            auto itLeft = std::begin(left);
            auto itRight = std::begin(right);
            while(itLeft != std::end(left) && itRight != std::end(right)) {

                // Discover type of token match
                auto match = matches(*itLeft, *itRight, environment, lineNumber);

                switch(match) {
                    case MatchType::Exact:
                    {
                        if(handleExact(left, right, itLeft, itRight)) {
                            continue;
                        }
                        return false;
                    }
                    case MatchType::SingleEq:
                    {
                        if(handleEq(left, right, itLeft, itRight)) {
                            continue;
                        }
                        return false;
                    }
                    case MatchType::DoubleEq:
                    {
                        if(handleEqEq(left, right, itLeft, itRight, environment, lineNumber)) {
                            continue;
                        }
                        return false;
                    }
                    case MatchType::QVar:
                    {
                        if(handleQVar(left, right, itLeft, itRight, environment,
                                      lineNumber)) {
                            continue;
                        }
                        return false;
                        
                    }
                    case MatchType::QQVar:
                    {
                        if(handleQQVar(left, right, itLeft, itRight, environment,
                                      lineNumber)) {
                            continue;
                        }
                        return false;
                    }
                    case MatchType::NoMatch:
                    {
                        return false;
                    }
                }
            }
            return true;
        }

    }

    MatchesExpressionEvaluator::MatchesExpressionEvaluator(MatchesExpression expression)
      : m_expression(std::move(expression))
    {
    }
    Type MatchesExpressionEvaluator::evaluate(Environment & environment) const 
    {
        auto const left = m_expression.getLeftExpression();
        auto const evalLeft = left->getEvaluator()->evaluate(environment);
        if(evalLeft.m_descriptor != TypeDescriptor::List) {
            throw LanguageException("Left expression not a list",
                                    m_expression.getLineNumber());
        }
        auto const right = m_expression.getRightExpression();
        auto const evalRight = right->getEvaluator()->evaluate(environment);
        if(evalRight.m_descriptor != TypeDescriptor::List) {
            throw LanguageException("Right expression not a list",
                                    m_expression.getLineNumber());
        }

        auto const leftList = std::get<std::vector<Type>>(evalLeft.m_variantType);
        auto const rightList = std::get<std::vector<Type>>(evalRight.m_variantType);

        // Simple case -- direct match
        if(leftList == rightList) {
            return {TypeDescriptor::Bool, true};
        }
        // Pattern match
        else if(listMatch(leftList, rightList, environment, m_expression.getLineNumber())) {
            return {TypeDescriptor::Bool, true};
        } 
        // No match
        return {TypeDescriptor::Bool, false};
    }
}