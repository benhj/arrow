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
          , m_index(0)
        {
        }

        void parse()
        {
            while(notAtEnd()) {
                if(m_tokens[m_index].lexeme == Lexeme::GENERIC_STRING) {
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
        int m_index;
        std::vector<std::shared_ptr<Statement>> m_statements;

        bool notAtEnd() const
        {
            return m_index < m_tokens.size();
        }

        void advanceTokenIterator()
        {
            ++m_index;
        }

        std::shared_ptr<Expression> parseIdentifierExpression(int const index)
        {
            auto exp = std::make_shared<IdentifierExpression>();
            exp->withIntToken(m_tokens[index]);
            return exp;
        }

        std::shared_ptr<Expression> parseLiteralIntExpression(int const index)
        {
            auto exp = std::make_shared<LiteralIntExpression>();
            exp->withIntToken(m_tokens[index]);
            return exp;
        }

        std::shared_ptr<Expression> parseLiteralRealExpression(int const index)
        {
            auto exp = std::make_shared<LiteralRealExpression>();
            exp->withRealToken(m_tokens[index]);
            return exp;
        }

        std::shared_ptr<Expression> parseLiteralStringExpression(int const index)
        {
            auto exp = std::make_shared<LiteralStringExpression>();
            exp->withStringToken(m_tokens[index]);
            return exp;
        }

        std::shared_ptr<Expression> parseOperatorExpression(int const index)
        {
            auto exp = std::make_shared<OperatorExpression>();
            exp->withOperator(m_tokens[index]);

            // Parse expression on left of operator.
            // Note the false is to make sure we don't
            // check the subsequent value for an operatos
            auto leftExpression = parseExpression(index - 1, false);
            if(leftExpression) {

                // Parse expression on right of operator
                auto rightExpression = parseExpression(index + 1);
                if(rightExpression) {
                    exp->withLeft(std::move(leftExpression));
                    exp->withRight(std::move(rightExpression));
                    return exp;
                }
            }
            return nullptr;
        }

        std::shared_ptr<Expression> parseGroupedExpression(int const index)
        {
            auto exp = std::make_shared<GroupedExpression>();
            auto expression = parseExpression(index);
            if(expression) {
                exp->withExpression(std::move(expression));
                return exp;
            }
            return nullptr;
        }

        std::shared_ptr<ListExpression> parseListExpression(int index)
        {
            auto listExp = std::make_shared<ListExpression>();
            while(m_tokens[index].lexeme != Lexeme::CLOSE_SQUARE) {
                std::shared_ptr<Expression> exp;
                if(m_tokens[index].lexeme == Lexeme::OPEN_SQUARE) {
                    advanceTokenIterator();
                    auto embeddedExp = parseListExpression(index + 1);
                    index += embeddedExp->getPartsCount();
                    exp = embeddedExp;
                } else {
                    exp = parseExpression(index);
                }
                if(exp) {
                    listExp->addPart(std::move(exp));
                }
                ++index;
            }
            advanceTokenIterator();
            return listExp;
        }

        std::shared_ptr<Expression> parseExpression(int const index,
                                                    bool checkOperator = true)
        {
            if (checkOperator && isOperator(m_tokens[index + 1].lexeme)) {
                advanceTokenIterator();
                return parseOperatorExpression(index + 1);
            } else if(m_tokens[index].lexeme == Lexeme::OPEN_PAREN) {
                advanceTokenIterator();
                auto expression = parseGroupedExpression(index + 1);
                advanceTokenIterator();
                if(m_tokens[m_index].lexeme != Lexeme::CLOSE_PAREN) {
                    // unbalanced
                    return nullptr;
                }
                // Check for additional expression parts
                if(isOperator(m_tokens[m_index + 1].lexeme)) {
                    advanceTokenIterator();
                    auto exp = std::make_shared<OperatorExpression>();
                    exp->withOperator(m_tokens[m_index]);
                    if(!expression) {
                        return nullptr;
                    }
                    exp->withLeft(expression);
                    auto right = parseExpression(m_index + 1);
                    if(!right) {
                        return nullptr;
                    }
                    exp->withRight(right);
                    return exp;
                }
                return expression;
            } else if(m_tokens[index].lexeme == Lexeme::OPEN_SQUARE) {
                advanceTokenIterator();
                return parseListExpression(index + 1);
            }  else if(m_tokens[index].lexeme == Lexeme::INTEGER_NUM) {
                advanceTokenIterator();
                return parseLiteralIntExpression(index);
            } else if(m_tokens[index].lexeme == Lexeme::REAL_NUM) {
                advanceTokenIterator();
                return parseLiteralRealExpression(index);
            } else if(m_tokens[index].lexeme == Lexeme::GENERIC_STRING) {
                advanceTokenIterator();
                return parseIdentifierExpression(index);
            } else if(m_tokens[index].lexeme == Lexeme::LITERAL_STRING) {
                advanceTokenIterator();
                return parseLiteralStringExpression(index);
            }
            return nullptr;
        }

        std::shared_ptr<Statement> parseArrowStatement()
        {
            auto arrowStatement = std::make_shared<ArrowStatement>();
            arrowStatement->withToken(m_tokens[m_index]);
            auto expression = parseExpression(m_index + 1);
            if(expression) {
                arrowStatement->withExpression(std::move(expression));
                advanceTokenIterator();
                if(notAtEnd()) {
                    std::cout<<m_tokens[m_index].lexeme<<std::endl;
                    if(m_tokens[m_index].lexeme != Lexeme::ARROW) {
                        return nullptr;
                    }
                    advanceTokenIterator();
                    if(notAtEnd()) {
                        if(m_tokens[m_index].lexeme == Lexeme::GENERIC_STRING) {
                            arrowStatement->withIdentifier(m_tokens[m_index]);
                            advanceTokenIterator();
                            if(notAtEnd()) {
                                if(m_tokens[m_index].lexeme == Lexeme::SEMICOLON) {
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
