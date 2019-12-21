/// (c) Ben Jones 2019 - present

/// This
#include "ExpressionParser.hpp"

/// Expressions
#include "expressions/BooleanExpression.hpp"
#include "expressions/BracedExpressionCollectionExpression.hpp"
#include "expressions/DoubleEqualExpression.hpp"
#include "expressions/ExpressionCollectionExpression.hpp"
#include "expressions/FunctionExpression.hpp"
#include "expressions/GroupedExpression.hpp"
#include "expressions/HatHatStringExpression.hpp"
#include "expressions/HatStringExpression.hpp"
#include "expressions/IdentifierExpression.hpp"
#include "expressions/IndexExpression.hpp"
#include "expressions/ListExpression.hpp"
#include "expressions/ListWordExpression.hpp"
#include "expressions/LiteralCharExpression.hpp"
#include "expressions/LiteralIntExpression.hpp"
#include "expressions/LiteralRealExpression.hpp"
#include "expressions/LiteralStringExpression.hpp"
#include "expressions/MatchesExpression.hpp"
#include "expressions/MathExpression.hpp"
#include "expressions/OperatorExpression.hpp"
#include "expressions/QQStringExpression.hpp"
#include "expressions/QStringExpression.hpp"
#include "expressions/RandomFunctionExpression.hpp"
#include "expressions/SingleEqualExpression.hpp"
#include "expressions/StringInputExpression.hpp"
#include "expressions/SystemCommandExpression.hpp"

/// Other
#include "LanguageException.hpp"
#include "lexer/Lexeme.hpp"
#include <algorithm>
#include <functional>
#include <utility>

namespace arrow {

    ExpressionParser::ExpressionParser(TokenManager & tm)
      : m_tm(tm)
    {
    }

    std::shared_ptr<Expression> ExpressionParser::parseIdentifierExpression()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<IdentifierExpression>(ln);
        exp->withIdentifierToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseIndexExpression()
    {
        if(m_tm.nextToken().lexeme != Lexeme::OPEN_SQUARE) {
            return nullptr;
        }
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<IndexExpression>(ln);

        exp->withIdentifierToken(m_tm.currentToken());
        m_tm.advanceTokenIterator();
        m_tm.advanceTokenIterator();
        if(m_tm.currentToken().lexeme == Lexeme::CLOSE_SQUARE) {
            throw LanguageException("No index supplied", ln);
        }
        auto innerExpression = parseExpression();
        if(!innerExpression) {
            return nullptr;
        }
        exp->withIndexExpression(std::move(innerExpression));
        m_tm.advanceTokenIterator();
        if(m_tm.currentToken().lexeme != Lexeme::CLOSE_SQUARE) {
            return nullptr;
        }
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseListWordExpression()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<ListWordExpression>(ln);
        exp->withWordToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseLiteralIntExpression()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<LiteralIntExpression>(ln);
        exp->withIntToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseLiteralRealExpression()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<LiteralRealExpression>(ln);
        exp->withRealToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseLiteralStringExpression()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<LiteralStringExpression>(ln);
        exp->withStringToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseLiteralCharExpression()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<LiteralCharExpression>(ln);
        exp->withStringToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseHatStringExpression()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<HatStringExpression>(ln);
        exp->withHatStringToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseHatHatStringExpression()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<HatHatStringExpression>(ln);
        exp->withHatHatStringToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseQStringExpression()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<QStringExpression>(ln);
        exp->withQStringToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseQQStringExpression()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<QQStringExpression>(ln);
        exp->withQQStringToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseSingleEqualExpression()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<SingleEqualExpression>(ln);
        exp->withToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseDoubleEqualExpression()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<DoubleEqualExpression>(ln);
        exp->withToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseMatchesExpression()
    {
        auto const ln = m_tm.currentToken().lineNumber;

        auto store = m_tm.retrieveIt();
        auto left = parseListExpression();
        if(!left) {
            m_tm.revert(store);
            left = parseIdentifierExpression();
            if(!left) {
                return nullptr;
            }
        }
        m_tm.advanceTokenIterator();
        if(m_tm.currentToken().raw != "matches") {
            return nullptr;
        }
        m_tm.advanceTokenIterator();
        store = m_tm.retrieveIt();
        auto right = parseListExpression();
        if(!right) {
            m_tm.revert(store);
            right = parseIdentifierExpression();
            if(!right) {
                return nullptr;
            }
        }
        auto matchesExpression = std::make_shared<MatchesExpression>(ln);
        matchesExpression->withLeftExpression(std::move(left));
        matchesExpression->withRightExpression(std::move(right));
        return matchesExpression;
    }

    std::shared_ptr<Expression> ExpressionParser::parseOperatorExpression()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<OperatorExpression>(ln);
        auto leftExpression = parseExpression(false);
        if(!leftExpression) {
           return nullptr;
        }
        exp->withLeft(std::move(leftExpression));
        m_tm.advanceTokenIterator();

        exp->withOperator(m_tm.currentToken());
        m_tm.advanceTokenIterator();

        auto rightExpression = parseExpression();
        if(!rightExpression) {
            return nullptr;
        }
        exp->withRight(std::move(rightExpression));
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseBooleanExpression()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<BooleanExpression>(ln);
        auto leftExpression = parseExpression(false);
        if(!leftExpression) {
           return nullptr;
        }
        exp->withLeft(std::move(leftExpression));
        m_tm.advanceTokenIterator();

        exp->withOperator(m_tm.currentToken());
        m_tm.advanceTokenIterator();

        auto rightExpression = parseExpression();
        if(!rightExpression) {
            return nullptr;
        }
        exp->withRight(std::move(rightExpression));
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseMathExpression()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<MathExpression>(ln);
        auto leftExpression = parseExpression(false);
        if(!leftExpression) {
           return nullptr;
        }
        exp->withLeft(std::move(leftExpression));
        m_tm.advanceTokenIterator();

        exp->withOperator(m_tm.currentToken());
        m_tm.advanceTokenIterator();

        auto rightExpression = parseExpression();
        if(!rightExpression) {
            return nullptr;
        }
        exp->withRight(std::move(rightExpression));
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseGroupedExpression()
    {
        // Skip over paren
        m_tm.advanceTokenIterator();

        // Parse expression
        auto expression = parseExpression();
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<GroupedExpression>(ln);
        if(expression) {

            exp->withExpression(std::move(expression));

            // Skip to get to next paren
            m_tm.advanceTokenIterator();

            if(m_tm.currentToken().lexeme != Lexeme::CLOSE_PAREN) {
                return nullptr;
            }

            // Check for additional parts
            if(isMathOperator(m_tm.nextToken().lexeme)) {
                auto extra = std::make_shared<MathExpression>(ln);
                extra->withLeft(std::move(exp));
                m_tm.advanceTokenIterator();
                extra->withOperator(m_tm.currentToken());
                m_tm.advanceTokenIterator();
                auto right = parseExpression();
                if(!right) {
                    return nullptr;
                }
                extra->withRight(std::move(right));
                return extra;
            } else if(isBooleanOperator(m_tm.nextToken().lexeme)) {
                auto extra = std::make_shared<BooleanExpression>(ln);
                extra->withLeft(std::move(exp));
                m_tm.advanceTokenIterator();
                extra->withOperator(m_tm.currentToken());
                m_tm.advanceTokenIterator();
                auto right = parseExpression();
                if(!right) {
                    return nullptr;
                }
                extra->withRight(std::move(right));
                return extra;
            }
            return exp;
        }
        return nullptr;
    }

    std::shared_ptr<Expression> ExpressionParser::parseListExpression()
    {

        // Skip over open square bracket
        m_tm.advanceTokenIterator();
        auto const ln = m_tm.currentToken().lineNumber;
        auto listExp = std::make_shared<ListExpression>(ln);
        while(m_tm.currentToken().lexeme != Lexeme::CLOSE_SQUARE) {
            // Note, use partListExpressionType here rather
            // than parseExpression to make it easier to
            // distinguish between plain words and identifiers
            // both of which are generic string lexeme types.
            auto exp = parseListExpressionType();
            if(!exp) { return nullptr; }
            listExp->addPart(std::move(exp));
            m_tm.advanceTokenIterator();
        }
        return listExp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseFunctionExpression()
    {
        if(m_tm.currentToken().lexeme != Lexeme::GENERIC_STRING) { return nullptr; }
        if(m_tm.nextToken().lexeme != Lexeme::OPEN_PAREN) { return nullptr; }
        auto const ln = m_tm.currentToken().lineNumber;
        if(m_tm.currentToken().raw == "random") {
            auto functionExpression = std::make_shared<RandomFunctionExpression>(ln);
            m_tm.advanceTokenIterator();
            auto expression = parseExpression();
            if(!expression) { return nullptr; }
            functionExpression->withExpression(std::move(expression));
            return functionExpression;
        } else if(m_tm.currentToken().raw == "input") {
            auto stringInputExpression = std::make_shared<StringInputExpression>(ln);
            m_tm.advanceTokenIterator();
            auto expression = parseExpression();
            if(!expression) { return nullptr; }
            stringInputExpression->withExpression(std::move(expression));
            return stringInputExpression;
        } else if(m_tm.currentToken().raw == "exec") {
            auto systemCommandExpression = std::make_shared<SystemCommandExpression>(ln);
            m_tm.advanceTokenIterator();
            auto expression = parseExpression();
            if(!expression) { return nullptr; }
            systemCommandExpression->withExpression(std::move(expression));
            return systemCommandExpression;
        }  else {
            auto functionExpression = std::make_shared<FunctionExpression>(ln);
            functionExpression->withFunctionNameToken(m_tm.currentToken());
            m_tm.advanceTokenIterator();
            auto collection = parseExpressionCollectionExpression();
            if(!collection) { return nullptr; }
            functionExpression->withExpressionCollection(std::move(collection));
            return functionExpression;
        }
    }

    std::shared_ptr<Expression> 
    ExpressionParser::parseExpressionCollectionExpression(bool const identifierOnly)
    {
        if(m_tm.currentToken().lexeme != Lexeme::OPEN_PAREN) { return nullptr; }
        auto const ln = m_tm.currentToken().lineNumber;
        auto expression = std::make_shared<ExpressionCollectionExpression>(ln);
        m_tm.advanceTokenIterator();
        while(m_tm.currentToken().lexeme != Lexeme::CLOSE_PAREN) {
            std::shared_ptr<Expression> exp;
            if(identifierOnly) {
                if(m_tm.currentToken().lexeme != Lexeme::GENERIC_STRING) {
                    exp = nullptr; // error
                } else {
                    exp = parseListWordExpression();
                }
            } else {
                exp = parseExpression();
            }

            if(!exp) { 
                // When there are no parameters
                if(m_tm.currentToken().lexeme == Lexeme::CLOSE_PAREN) {
                    break;
                }
                return nullptr; 
            }
            expression->addExpression(std::move(exp));
            m_tm.advanceTokenIterator();
            if(m_tm.currentToken().lexeme == Lexeme::COMMA) {
                m_tm.advanceTokenIterator();
            }
        }
        return expression;
    }

    std::shared_ptr<Expression> 
    ExpressionParser::parseBracedExpressionCollectionExpression()
    {
        if(m_tm.currentToken().lexeme != Lexeme::OPEN_CURLY) { return nullptr; }
        auto const ln = m_tm.currentToken().lineNumber;
        auto expression = std::make_shared<BracedExpressionCollectionExpression>(ln);
        m_tm.advanceTokenIterator();
        while(m_tm.currentToken().lexeme != Lexeme::CLOSE_CURLY) {
            std::shared_ptr<Expression> exp = parseExpression();
            
            if(!exp) { 
                return nullptr; 
            }
            expression->addExpression(std::move(exp));
            m_tm.advanceTokenIterator();
            if(m_tm.currentToken().lexeme == Lexeme::COMMA) {
                m_tm.advanceTokenIterator();
            }
        }
        return expression;
    }

    std::shared_ptr<Expression> ExpressionParser::parseListExpressionType()
    {
        if(m_tm.currentToken().lexeme == Lexeme::GENERIC_STRING) {
            return parseListWordExpression();
        } else if(m_tm.currentToken().lexeme == Lexeme::HAT_HAT_STRING) {
            return parseHatHatStringExpression();
        } else if(m_tm.currentToken().lexeme == Lexeme::HAT_STRING) {
            return parseHatStringExpression();
        } else if(m_tm.currentToken().lexeme == Lexeme::Q_Q_STRING) {
            return parseQQStringExpression();
        } else if(m_tm.currentToken().lexeme == Lexeme::Q_STRING) {
            return parseQStringExpression();
        } else if(m_tm.currentToken().lexeme == Lexeme::OPEN_SQUARE) {
            return parseListExpression();
        } else if(m_tm.currentToken().lexeme == Lexeme::EQUAL) {
            return parseSingleEqualExpression();
        } else if(m_tm.currentToken().lexeme == Lexeme::EQUAL_EQUAL) {
            return parseDoubleEqualExpression();
        } else if(m_tm.currentToken().lexeme == Lexeme::INTEGER_NUM) {
            return parseLiteralIntExpression();
        } else if(m_tm.currentToken().lexeme == Lexeme::REAL_NUM) {
            return parseLiteralRealExpression();
        } 
        return nullptr;
    }

    std::shared_ptr<Expression> ExpressionParser::parseExpression(bool checkOperator)
    {
        if(m_tm.tokenPlusOneNotAtEnd()) {
            if (checkOperator && isBooleanOperator(m_tm.nextToken().lexeme) &&
                m_tm.currentToken().lexeme != Lexeme::OPEN_SQUARE) {
                return parseBooleanExpression();
            } else if(checkOperator && isMathOperator(m_tm.nextToken().lexeme))  {
                return parseMathExpression();
            } else if(m_tm.currentToken().lexeme == Lexeme::OPEN_PAREN) {

                auto store = m_tm.retrieveIt();
                auto exp = parseGroupedExpression();
                if(!exp) {
                    m_tm.revert(store);
                    // try (a, b, c) collection
                    return parseExpressionCollectionExpression();
                }
                return exp;
            } else if(m_tm.currentToken().lexeme == Lexeme::OPEN_CURLY) {
                return parseBracedExpressionCollectionExpression();
            } else if(m_tm.currentToken().lexeme == Lexeme::OPEN_SQUARE) {
                auto store = m_tm.retrieveIt();
                auto exp = parseMatchesExpression();
                if(!exp) {
                    m_tm.revert(store);
                    return parseListExpression();
                }
                return exp;
            } else if(m_tm.currentToken().lexeme == Lexeme::INTEGER_NUM) {
                return parseLiteralIntExpression();
            } else if(m_tm.currentToken().lexeme == Lexeme::REAL_NUM) {
                return parseLiteralRealExpression();
            } else if(m_tm.currentToken().lexeme == Lexeme::GENERIC_STRING) {
                auto store = m_tm.retrieveIt();
                auto exp = parseFunctionExpression();
                if(!exp) {
                    m_tm.revert(store);
                    auto store = m_tm.retrieveIt();
                    exp = parseIndexExpression();
                    if(!exp) {
                        m_tm.revert(store);
                        store = m_tm.retrieveIt();
                        exp = parseMatchesExpression();
                        if(!exp) {
                            m_tm.revert(store);
                            return parseIdentifierExpression();
                        }
                    }
                }
                return exp;
            } else if(m_tm.currentToken().lexeme == Lexeme::LITERAL_STRING) {
                return parseLiteralStringExpression();
            } else if(m_tm.currentToken().lexeme == Lexeme::LITERAL_CHAR) {
                return parseLiteralCharExpression();
            }
        }
        return nullptr;
    }
}
