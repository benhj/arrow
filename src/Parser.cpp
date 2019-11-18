#include "Parser.hpp"

#include "ArrowStatement.hpp"
#include "ArrowlessStatement.hpp"
#include "GroupedExpression.hpp"
#include "HatHatStringExpression.hpp"
#include "HatStringExpression.hpp"
#include "IdentifierExpression.hpp"
#include "Lexeme.hpp"
#include "ListExpression.hpp"
#include "LiteralIntExpression.hpp"
#include "LiteralRealExpression.hpp"
#include "LiteralStringExpression.hpp"
#include "OperatorExpression.hpp"
#include "QQStringExpression.hpp"
#include "QStringExpression.hpp"

#include <utility>

namespace jasl {
    
    Parser::Parser(std::vector<Token> tokens)
      : m_tokens(std::move(tokens))
      , m_current(std::begin(m_tokens))
    {
    }

    void Parser::parse()
    {
        while(notAtEnd()) {
            buildStatement();
            advanceTokenIterator();
        }
    }

    std::vector<std::shared_ptr<Statement>> Parser::getStatements() const
    {
        return m_statements;
    }

    void Parser::buildStatement()
    {
        if(currentToken().lexeme == Lexeme::GENERIC_STRING) {
            {
                auto store = m_current;
                auto statement = parseArrowStatement();
                if(statement) { 
                    m_statements.emplace_back(std::move(statement));
                    return;
                }
                // Revert token iterator state in case of failure
                m_current = store;
            }
            {
                auto store = m_current;
                auto statement = parseArrowlessStatement();
                if(statement) { 
                    m_statements.emplace_back(std::move(statement));
                    return;
                }
                // Revert token iterator state in case of failure
                m_current = store;
            }
        }
    }

    bool Parser::notAtEnd() const
    {
        return m_current != std::end(m_tokens);
    }

    void Parser::advanceTokenIterator()
    {
        ++m_current;
    }

    Token Parser::currentToken() const
    {
        return *m_current;
    }

    Token Parser::nextToken() const
    {
        return *(m_current + 1);
    }

    std::shared_ptr<Expression> Parser::parseIdentifierExpression()
    {
        auto exp = std::make_shared<IdentifierExpression>();
        exp->withIntToken(currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseLiteralIntExpression()
    {
        auto exp = std::make_shared<LiteralIntExpression>();
        exp->withIntToken(currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseLiteralRealExpression()
    {
        auto exp = std::make_shared<LiteralRealExpression>();
        exp->withRealToken(currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseLiteralStringExpression()
    {
        auto exp = std::make_shared<LiteralStringExpression>();
        exp->withStringToken(currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseHatStringExpression()
    {
        auto exp = std::make_shared<HatStringExpression>();
        exp->withHatStringToken(currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseHatHatStringExpression()
    {
        auto exp = std::make_shared<HatHatStringExpression>();
        exp->withHatHatStringToken(currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseQStringExpression()
    {
        auto exp = std::make_shared<QStringExpression>();
        exp->withQStringToken(currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseQQStringExpression()
    {
        auto exp = std::make_shared<QQStringExpression>();
        exp->withQQStringToken(currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseOperatorExpression()
    {
        auto exp = std::make_shared<OperatorExpression>();
        auto leftExpression = parseExpression(false);
        if(!leftExpression) {
           return nullptr;
        }
        exp->withLeft(std::move(leftExpression));
        advanceTokenIterator();

        exp->withOperator(currentToken());
        advanceTokenIterator();

        auto rightExpression = parseExpression();
        if(!rightExpression) {
            return nullptr;
        }
        exp->withRight(std::move(rightExpression));
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseGroupedExpression()
    {
        // Skip over paren
        advanceTokenIterator();

        // Parse expression
        auto expression = parseExpression();
        auto exp = std::make_shared<GroupedExpression>();
        if(expression) {

            exp->withExpression(std::move(expression));

            // Skip to get to next paren
            advanceTokenIterator();

            if(currentToken().lexeme != Lexeme::CLOSE_PAREN) {
                return nullptr;
            }

            // Check for additional parts
            if(isOperator(nextToken().lexeme)) {
                auto extra = std::make_shared<OperatorExpression>();
                extra->withLeft(std::move(exp));
                advanceTokenIterator();
                extra->withOperator(currentToken());
                advanceTokenIterator();
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

    std::shared_ptr<Expression> Parser::parseListExpression()
    {

        // Skip over open square bracket
        advanceTokenIterator();
        auto listExp = std::make_shared<ListExpression>();
        while(currentToken().lexeme != Lexeme::CLOSE_SQUARE) {
            auto exp = parseExpression();
            if(!exp) { return nullptr; }
            listExp->addPart(std::move(exp));
            advanceTokenIterator();
        }
        return listExp;
    }

    std::shared_ptr<Expression> Parser::parseExpression(bool checkOperator)
    {
        if (checkOperator && isOperator(nextToken().lexeme)) {
            return parseOperatorExpression();
        } else if(currentToken().lexeme == Lexeme::OPEN_PAREN) {
            return parseGroupedExpression();
        } else if(currentToken().lexeme == Lexeme::OPEN_SQUARE) {
            return parseListExpression();
        } else if(currentToken().lexeme == Lexeme::INTEGER_NUM) {
            return parseLiteralIntExpression();
        } else if(currentToken().lexeme == Lexeme::REAL_NUM) {
            return parseLiteralRealExpression();
        } else if(currentToken().lexeme == Lexeme::GENERIC_STRING) {
            return parseIdentifierExpression();
        } else if(currentToken().lexeme == Lexeme::LITERAL_STRING) {
            return parseLiteralStringExpression();
        } else if(currentToken().lexeme == Lexeme::HAT_HAT_STRING) {
            return parseHatHatStringExpression();
        } else if(currentToken().lexeme == Lexeme::HAT_STRING) {
            return parseHatStringExpression();
        } else if(currentToken().lexeme == Lexeme::Q_Q_STRING) {
            return parseQQStringExpression();
        } else if(currentToken().lexeme == Lexeme::Q_STRING) {
            return parseQStringExpression();
        }
        return nullptr;
    }

    std::shared_ptr<Statement> Parser::parseArrowStatement()
    {
        auto arrowStatement = std::make_shared<ArrowStatement>();
        arrowStatement->withToken(currentToken());
        advanceTokenIterator();
        
        auto expression = parseExpression();
        if(expression) {
            arrowStatement->withExpression(std::move(expression));
            advanceTokenIterator();
            if(notAtEnd()) {
                if(currentToken().lexeme != Lexeme::ARROW) {
                    return nullptr;
                }
                advanceTokenIterator();
                if(notAtEnd()) {
                    if(currentToken().lexeme == Lexeme::GENERIC_STRING) {
                        arrowStatement->withIdentifier(currentToken());
                        advanceTokenIterator();
                        if(notAtEnd()) {
                            if(currentToken().lexeme == Lexeme::SEMICOLON) {
                                return arrowStatement;
                            }
                        }
                    }
                }
            }
        }
        return nullptr;
    }

    std::shared_ptr<Statement> Parser::parseArrowlessStatement()
    {
        auto arrowlessStatement = std::make_shared<ArrowlessStatement>();
        arrowlessStatement->withToken(currentToken());
        advanceTokenIterator();
        auto expression = parseExpression();
        if(expression) {
            arrowlessStatement->withExpression(std::move(expression));
            advanceTokenIterator();
            if(notAtEnd()) {
                if(currentToken().lexeme == Lexeme::SEMICOLON) {
                    return arrowlessStatement;
                }
            }
        }
        return nullptr;
    }
}
