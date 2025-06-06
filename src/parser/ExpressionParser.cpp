/// (c) Ben Jones 2019 - present

/// This
#include "ExpressionParser.hpp"
#include "BuiltInFunctionExpressionParser.hpp"

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
#include "expressions/NegativeNumberExpression.hpp"
#include "expressions/MatchesExpression.hpp"
#include "expressions/MathExpression.hpp"
#include "expressions/OperatorExpression.hpp"
#include "expressions/QQStringExpression.hpp"
#include "expressions/QStringExpression.hpp"
#include "expressions/SingleEqualExpression.hpp"
#include "builtin/BuiltInFunctionExpression.hpp"

/// Other
#include "LanguageException.hpp"
#include "lexer/Lexeme.hpp"
#include <algorithm>
#include <functional>
#include <utility>

namespace arrow {

    namespace {
        int precedence(Lexeme const operatorLexeme) {
            switch(operatorLexeme) {
                case Lexeme::AND:
                case Lexeme::OR:
                    return 0;
                case Lexeme::XOR:
                    return 1;
                case Lexeme::EQUAL:
                case Lexeme::NOT_EQUAL:
                    return 2;
                case Lexeme::LESS:
                case Lexeme::GREATER:
                case Lexeme::LESS_EQUAL:
                case Lexeme::GREATER_EQUAL:
                    return 3; 
                case Lexeme::PLUS:
                case Lexeme::MINUS:
                    return 4;
                case Lexeme::STAR:
                case Lexeme::DIVIDE:
                case Lexeme::PERCENT:
                    return 5;
                default:
                    // shouldn't ever happen
                    throw std::logic_error("Incorrect operator type");
            }
        }
    }

    ExpressionParser::ExpressionParser(TokenManager & tm)
      : m_tm(tm)
      , m_builtinsParser(std::make_shared<BuiltInFunctionExpressionParser>(m_tm))
    {
    }

    std::shared_ptr<Expression> ExpressionParser::parseIdentifierExpression()
    {
        if(m_tm.currentToken().lexeme != Lexeme::GENERIC_STRING) {
            return nullptr;
        }
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<IdentifierExpression>(ln);
        exp->withIdentifierToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseIndexExpression()
    {
        if(m_tm.currentToken().lexeme != Lexeme::GENERIC_STRING) {
            return nullptr;
        }
        if(m_tm.nextToken().lexeme != Lexeme::COLON) {
            return nullptr;
        }
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<IndexExpression>(ln);

        exp->withIdentifierToken(m_tm.currentToken());
        m_tm.advanceTokenIterator();
        m_tm.advanceTokenIterator();
        auto innerExpression = parseUnaryExpression();
        if(!innerExpression) {
            return nullptr;
        } else {
            exp->withIndexExpression(std::move(innerExpression));
        }
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseListWordExpression()
    {
        if(m_tm.currentToken().lexeme != Lexeme::GENERIC_STRING) {
            return nullptr;
        }
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<ListWordExpression>(ln);
        exp->withWordToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseLiteralIntExpression()
    {
        if(m_tm.currentToken().lexeme != Lexeme::INTEGER_NUM) {
            return nullptr;
        }
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<LiteralIntExpression>(ln);
        exp->withIntToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseLiteralRealExpression()
    {
        if(m_tm.currentToken().lexeme != Lexeme::REAL_NUM) {
            return nullptr;
        }
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<LiteralRealExpression>(ln);
        exp->withRealToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseLiteralStringExpression()
    {
        if(m_tm.currentToken().lexeme != Lexeme::LITERAL_STRING) {
            return nullptr;
        }
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<LiteralStringExpression>(ln);
        exp->withStringToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseLiteralCharExpression()
    {
        if(m_tm.currentToken().lexeme != Lexeme::LITERAL_CHAR) {
            return nullptr;
        }
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<LiteralCharExpression>(ln);
        exp->withStringToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseHatStringExpression()
    {
        if(m_tm.currentToken().lexeme != Lexeme::HAT_STRING) {
            return nullptr;
        }
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<HatStringExpression>(ln);
        exp->withHatStringToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseHatHatStringExpression()
    {
        if(m_tm.currentToken().lexeme != Lexeme::HAT_HAT_STRING) {
            return nullptr;
        }
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<HatHatStringExpression>(ln);
        exp->withHatHatStringToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseQStringExpression()
    {
        if(m_tm.currentToken().lexeme != Lexeme::Q_STRING) {
            return nullptr;
        }
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<QStringExpression>(ln);
        exp->withQStringToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseQQStringExpression()
    {
        if(m_tm.currentToken().lexeme != Lexeme::Q_Q_STRING) {
            return nullptr;
        }
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<QQStringExpression>(ln);
        exp->withQQStringToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseSingleEqualExpression()
    {
        if(m_tm.currentToken().lexeme != Lexeme::EQUAL) {
            return nullptr;
        }
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<SingleEqualExpression>(ln);
        exp->withToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseDoubleEqualExpression()
    {
        if(m_tm.currentToken().lexeme != Lexeme::EQUAL_EQUAL) {
            return nullptr;
        }
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<DoubleEqualExpression>(ln);
        exp->withToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseMatchesExpression()
    {

        if(m_tm.currentToken().lexeme != Lexeme::OPEN_SQUARE &&
           m_tm.currentToken().lexeme != Lexeme::GENERIC_STRING) {
            return nullptr;
        }

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

    std::shared_ptr<Expression>
    ExpressionParser::parseBooleanExpression(std::shared_ptr<Expression> left,
                                             std::optional<int> previousPrecedence)
    {
        if(!isBooleanOperator(m_tm.nextToken().lexeme)) {
            return nullptr;
        }

        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<BooleanExpression>(ln);
        exp->withLeft(std::move(left));

        auto const thisPrecedence = precedence(m_tm.nextToken().lexeme);
        if(previousPrecedence && *previousPrecedence >= thisPrecedence) {
            return left;
        }
        m_tm.advanceTokenIterator();
        exp->withOperator(m_tm.currentToken());
        m_tm.advanceTokenIterator();

        auto rightExpression = parseUnaryExpression();
        if(!rightExpression) {
            return nullptr;
        }
        auto moreBoolean = parseBooleanExpression(rightExpression, thisPrecedence);
        if(moreBoolean) {
            exp->withRight(std::move(moreBoolean));
        } else {
            // Is the RHS a math expression?
            // Note the expression passed in is the rightExpression which
            // will be the left-most part of the following math expression,
            // if indeed it is a math expression.
            auto mathExpr = parseMathExpression(rightExpression);
            if(mathExpr) {
                exp->withRight(std::move(mathExpr));
            } else {
                exp->withRight(std::move(rightExpression));
            }
        }

        // More boolean stuff?
        if(isBooleanOperator(m_tm.nextToken().lexeme)) {
            return parseBooleanExpression(exp);
        }
        return exp;
    }

    std::shared_ptr<Expression>
    ExpressionParser::parseMathExpression(std::shared_ptr<Expression> left,
                                          std::optional<int> previousPrecedence)
    {
        if(!isMathOperator(m_tm.nextToken().lexeme)) {
            return nullptr;
        }

        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<MathExpression>(ln);
        exp->withLeft(std::move(left));

        auto const thisPrecedence = precedence(m_tm.nextToken().lexeme);
        if(previousPrecedence && *previousPrecedence >= thisPrecedence) {
            return left;
        }
        m_tm.advanceTokenIterator();
        exp->withOperator(m_tm.currentToken());
        m_tm.advanceTokenIterator();

        auto rightExpression = parseUnaryExpression();
        if(!rightExpression) {
            return nullptr;
        }
        auto moreMath = parseMathExpression(rightExpression, thisPrecedence);
        if(moreMath) {
            exp->withRight(std::move(moreMath));
        } else {
            exp->withRight(std::move(rightExpression));
        }
        // More math stuff?
        if(isMathOperator(m_tm.nextToken().lexeme)) {
            return parseMathExpression(exp);
        }
        return exp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseGroupedExpression()
    {
        if(m_tm.currentToken().lexeme != Lexeme::OPEN_PAREN) {
            return nullptr;
        }

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
            return exp;
        }
        return nullptr;
    }

    std::shared_ptr<Expression> ExpressionParser::parseListExpression()
    {
        if(m_tm.currentToken().lexeme != Lexeme::OPEN_SQUARE) {
            return nullptr;
        }
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
            if(!exp) {
                return nullptr;
            }
            listExp->addPart(std::move(exp));
            m_tm.advanceTokenIterator();
        }
        return listExp;
    }

    std::shared_ptr<Expression> ExpressionParser::parseFunctionExpression()
    {
        if(m_tm.currentToken().lexeme != Lexeme::GENERIC_STRING) {
            return nullptr;
        }
        auto const ln = m_tm.currentToken().lineNumber;
        auto builtin = m_builtinsParser->parseExpression(m_tm.currentToken().raw);
        if(builtin) {
            m_tm.advanceTokenIterator();
            auto expression = parseExpressionCollectionExpression();
            if(!expression) {
                return nullptr;
            }
            // HAF!!! Needs improvement.
            dynamic_cast<BuiltInFunctionExpression*>(builtin.get())->setExpression(std::move(expression));
            return builtin;
        } else {
            auto functionExpression = std::make_shared<FunctionExpression>(ln);
            functionExpression->withFunctionNameToken(m_tm.currentToken());
            m_tm.advanceTokenIterator();
            auto collection = parseExpressionCollectionExpression();
            if(!collection) {
                return nullptr;
            }
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
        if(m_tm.currentToken().lexeme != Lexeme::OPEN_CURLY) {
            return nullptr;
        }
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

    std::shared_ptr<Expression>
    ExpressionParser::parseNegativeNumberExpression()
    {
        if(m_tm.currentToken().lexeme != Lexeme::MINUS) {
            return nullptr;
        }
        auto const ln = m_tm.currentToken().lineNumber;
        auto expression = std::make_shared<NegativeNumberExpression>(ln);
        m_tm.advanceTokenIterator();
        std::shared_ptr<Expression> expr = parseExpression(false /* == don't allow binary exp */);
        if(!expr) { 
            return nullptr; 
        }
        expression->withExpression(expr);
        return expression;
    }

    std::shared_ptr<Expression> ExpressionParser::parseListExpressionType()
    {
        static std::vector<std::function<std::shared_ptr<Expression>(ExpressionParser*)>> pvec{
            [](ExpressionParser* t){return t->parseListWordExpression();},
            [](ExpressionParser* t){return t->parseHatHatStringExpression();},
            [](ExpressionParser* t){return t->parseHatStringExpression();},
            [](ExpressionParser* t){return t->parseQStringExpression();},
            [](ExpressionParser* t){return t->parseQQStringExpression();},
            [](ExpressionParser* t){return t->parseListExpression();},
            [](ExpressionParser* t){return t->parseSingleEqualExpression();},
            [](ExpressionParser* t){return t->parseDoubleEqualExpression();},
            [](ExpressionParser* t){return t->parseLiteralIntExpression();},
            [](ExpressionParser* t){return t->parseLiteralRealExpression();}
        };
        for(auto const & p : pvec) {
            auto store = m_tm.retrieveIt();
            auto expression = p(this);
            if(expression) {
                return expression;
            }
            m_tm.revert(store);
        }
        return nullptr;
    }

    std::shared_ptr<Expression> 
    ExpressionParser::parseExpression(bool const allowBinaryExpressions,
                                      std::optional<int> prevPrecedence)
    {
        auto const expression = parseUnaryExpression();
        if(expression) {
            if(!allowBinaryExpressions) {
                return expression;
            }
            // If more tokens, might be able to match on a
            // boolean or math expression.
            if(m_tm.tokenPlusOneNotAtEnd()) {
                auto store = m_tm.retrieveIt();
                auto booleanEval = parseBooleanExpression(expression, prevPrecedence);
                if(booleanEval) {
                    return booleanEval;
                }
                m_tm.revert(store);
                store = m_tm.retrieveIt();
                auto mathEval = parseMathExpression(expression, prevPrecedence);
                if(mathEval) {
                    
                    // See if more boolean?
                    auto moreBoolean = parseBooleanExpression(mathEval);
                    if(!moreBoolean) {
                        return mathEval;
                    }
                    return moreBoolean;
                }
                m_tm.revert(store);
            }
            return expression;
        }
        return nullptr;
    }

    std::shared_ptr<Expression> ExpressionParser::parseUnaryExpression()
    {
        static std::vector<std::function<std::shared_ptr<Expression>(ExpressionParser*)>> pvec{
            [](ExpressionParser* t){return t->parseGroupedExpression();},
            [](ExpressionParser* t){return t->parseExpressionCollectionExpression();},
            [](ExpressionParser* t){return t->parseBracedExpressionCollectionExpression();},
            [](ExpressionParser* t){return t->parseMatchesExpression();},
            [](ExpressionParser* t){return t->parseListExpression();},
            [](ExpressionParser* t){return t->parseLiteralIntExpression();},
            [](ExpressionParser* t){return t->parseLiteralRealExpression();},
            [](ExpressionParser* t){return t->parseFunctionExpression();},
            [](ExpressionParser* t){return t->parseIndexExpression();},
            [](ExpressionParser* t){return t->parseIdentifierExpression();},
            [](ExpressionParser* t){return t->parseLiteralStringExpression();},
            [](ExpressionParser* t){return t->parseLiteralCharExpression();},
            [](ExpressionParser* t){return t->parseNegativeNumberExpression();}
        };
        for(auto const & p : pvec) {
            auto store = m_tm.retrieveIt();
            auto expression = p(this);
            if(expression) {
                return expression;
            }
            m_tm.revert(store);
        }
        return nullptr;
    }
}
