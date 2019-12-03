#include "MatchesStatementEvaluator.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>
#include <vector>

namespace arrow {

    namespace {
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

        MatchType matches(Type left, Type right)
        {
            if(left == right) {
                return MatchType::Exact;
            } else if(right.m_descriptor == TypeDescriptor::SingleEqual) {
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
                        std::vector<Type>::const_iterator & itRight)
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
            while(matches(*itLeft, *itRight) == MatchType::NoMatch) {
                ++itLeft;
                if(itLeft == std::end(left)) {
                    return false;
                }
            }
            ++itLeft;
            ++itRight;
            return true;
        }
        bool handleQVar(std::vector<Type> const & left,
                        std::vector<Type> const & right,
                        std::vector<Type>::const_iterator & itLeft,
                        std::vector<Type>::const_iterator & itRight,
                        Cache & cache,
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

            cache.add({Lexeme::GENERIC_STRING, var, lineNumber}, toStore);
            return true;
        }
        bool handleQQVar(std::vector<Type> const & left,
                         std::vector<Type> const & right,
                         std::vector<Type>::const_iterator & itLeft,
                         std::vector<Type>::const_iterator & itRight,
                         Cache & cache,
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
                cache.add({Lexeme::GENERIC_STRING, var, lineNumber},
                          {TypeDescriptor::List, std::move(elements)});
                return true;
            }

            // More tokens to process in left. We keep looping until we
            // find a match.
            auto matchType = matches(*itLeft, *itRight);
            while(matchType == MatchType::NoMatch) {
                elements.push_back(*itLeft);
                ++itLeft;
                if(itLeft == std::end(left)) {
                    return false;
                }
                matchType = matches(*itLeft, *itRight);
            }

            ++itLeft;
            ++itRight;
            cache.add({Lexeme::GENERIC_STRING, var, lineNumber},
                      {TypeDescriptor::List, std::move(elements)});
            return true;
        }

        bool listMatch(std::vector<Type> const & left,
                       std::vector<Type> const & right,
                       Cache & cache,
                       long const lineNumber)
        {
            auto itLeft = std::begin(left);
            auto itRight = std::begin(right);
            while(itLeft != std::end(left) && itRight != std::end(right)) {

                // Discover type of token match
                auto match = matches(*itLeft, *itRight);

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
                        if(handleEqEq(left, right, itLeft, itRight)) {
                            continue;
                        }
                        return false;
                    }
                    case MatchType::QVar:
                    {
                        if(handleQVar(left, right, itLeft, itRight, cache,
                                      lineNumber)) {
                            continue;
                        }
                        return false;
                        
                    }
                    case MatchType::QQVar:
                    {
                        if(handleQQVar(left, right, itLeft, itRight, cache,
                                      lineNumber)) {
                            continue;
                        }
                        return false;
                    }
                    case MatchType::NoMatch:
                    {
                        // Edge case in which one must be a list rather than a string
                        if(itLeft->m_descriptor == TypeDescriptor::List &&
                           itRight->m_descriptor == TypeDescriptor::List) {
                            auto listFirst = std::get<std::vector<Type>>(itLeft->m_variantType);
                            auto listSecond = std::get<std::vector<Type>>(itRight->m_variantType);
                            if(listMatch(listFirst, listSecond, cache, lineNumber)) {
                                ++itLeft;
                                ++itRight;
                                continue;
                            }
                            
                        }
                    }
                    default: return false;
                }
            }
            return false;
        }

    }

    MatchesStatementEvaluator::MatchesStatementEvaluator(MatchesStatement statement)
      : m_statement(std::move(statement))
    {
    }
    void MatchesStatementEvaluator::evaluate(Cache & cache) const 
    {
        auto const left = m_statement.getLeftExpression();
        auto const evalLeft = left->getEvaluator()->evaluate(cache);
        if(evalLeft.m_descriptor != TypeDescriptor::List) {
            throw LanguageException("Left expression not a list",
                                    m_statement.getLineNumber());
        }
        auto const right = m_statement.getRightExpression();
        auto const evalRight = right->getEvaluator()->evaluate(cache);
        if(evalRight.m_descriptor != TypeDescriptor::List) {
            throw LanguageException("Right expression not a list",
                                    m_statement.getLineNumber());
        }

        auto const leftList = std::get<std::vector<Type>>(evalLeft.m_variantType);
        auto const rightList = std::get<std::vector<Type>>(evalRight.m_variantType);
        auto const identifier = m_statement.getIdentifier();

        // Simple case -- direct match
        if(leftList == rightList) {
            cache.add(identifier, {TypeDescriptor::Bool, true});
            return;
        }
        // Pattern match
        else if(listMatch(leftList, rightList, cache, m_statement.getLineNumber())) {
            cache.add(identifier, {TypeDescriptor::Bool, true});
            return;
        } 
        // No match
        else {
            cache.add(identifier, {TypeDescriptor::Bool, false});
        }
        
    }
}