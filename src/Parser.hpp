#pragma once

#include "ArrowStatement.hpp"
#include "IdentifierExpression.hpp"
#include "LiteralIntExpression.hpp"
#include "LiteralRealExpression.hpp"
#include "LiteralStringExpression.hpp"
#include "GroupedExpression.hpp"
#include "ListExpression.hpp"
#include "OperatorExpression.hpp"
#include "Lexeme.hpp"
#include "Token.hpp"
#include <vector>
#include <memory>

namespace jasl {
    
    class Parser
    {
      public:
        Parser(std::vector<Token> tokens)
          : m_tokens(std::move(tokens))
          , m_current(std::begin(m_tokens))
        {
        }

        void parse()
        {
            while(notAtEnd()) {
                if(currentToken().lexeme == Lexeme::GENERIC_STRING) {
                    auto statement = parseArrowStatement();
                    if(statement) { 
                        m_statements.emplace_back(std::move(statement));
                    }
                }
                advanceTokenIterator();
            }
        }

        std::vector<std::shared_ptr<Statement>> getStatements() const
        {
            return m_statements;
        }

     private:
        std::vector<Token> m_tokens;
        std::vector<Token>::iterator m_current;
        std::vector<std::shared_ptr<Statement>> m_statements;

        bool notAtEnd() const
        {
            return m_current != std::end(m_tokens);
        }

        void advanceTokenIterator()
        {
            ++m_current;
        }

        Token currentToken() const
        {
            return *m_current;
        }

        Token nextToken() const
        {
            return *(m_current + 1);
        }

        std::shared_ptr<Expression> parseIdentifierExpression()
        {
            auto exp = std::make_shared<IdentifierExpression>();
            exp->withIntToken(currentToken());
            return exp;
        }

        std::shared_ptr<Expression> parseLiteralIntExpression()
        {
            auto exp = std::make_shared<LiteralIntExpression>();
            exp->withIntToken(currentToken());
            return exp;
        }

        std::shared_ptr<Expression> parseLiteralRealExpression()
        {
            auto exp = std::make_shared<LiteralRealExpression>();
            exp->withRealToken(currentToken());
            return exp;
        }

        std::shared_ptr<Expression> parseLiteralStringExpression()
        {
            auto exp = std::make_shared<LiteralStringExpression>();
            exp->withStringToken(currentToken());
            return exp;
        }

        std::shared_ptr<Expression> parseOperatorExpression()
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

        std::shared_ptr<Expression> parseGroupedExpression()
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


        std::shared_ptr<Expression> parseListExpression()
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

        std::shared_ptr<Expression> parseExpression(bool checkOperator = true)
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
            }
            return nullptr;
        }

        std::shared_ptr<Statement> parseArrowStatement()
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
    };
}
