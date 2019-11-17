#pragma once

#include "ArrowStatement.hpp"
#include "LiteralIntExpression.hpp"
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

        std::shared_ptr<Expression> parseLiteralIntExpression(int const index)
        {
            auto exp = std::make_shared<LiteralIntExpression>();
            exp->withIntToken(m_tokens[index]);
            return exp;
        }
        std::shared_ptr<Expression> parseOperatorExpression(int const index)
        {
            auto exp = std::make_shared<OperatorExpression>();
            exp->withOperator(m_tokens[index]);

            // Parse expression on left of operator
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

        std::shared_ptr<Expression> parseExpression(int const index,
                                                    bool checkOperator = true)
        {
            if(checkOperator && isOperator(m_tokens[index + 1].lexeme)) {
                advanceTokenIterator();
                return parseOperatorExpression(index + 1);
            } else if(m_tokens[index].lexeme == Lexeme::INTEGER_NUM) {
                advanceTokenIterator();
                return parseLiteralIntExpression(index);
            }
            return nullptr;
        }

        std::shared_ptr<Statement> parseArrowStatement()
        {
            auto intStatement = std::make_shared<ArrowStatement>();
            intStatement->withToken(m_tokens[m_index]);
            auto expression = parseExpression(m_index + 1);

            if(expression) {
                intStatement->withExpression(std::move(expression));
                advanceTokenIterator();
                if(notAtEnd()) {
                    std::cout<<m_tokens[m_index].raw<<std::endl;
                    if(m_tokens[m_index].lexeme != Lexeme::ARROW) {
                        return nullptr;
                    }
                    advanceTokenIterator();

                    if(notAtEnd()) {
                        if(m_tokens[m_index].lexeme == Lexeme::GENERIC_STRING) {
                            intStatement->withIdentifier(m_tokens[m_index]);
                            advanceTokenIterator();
                            if(notAtEnd()) {
                                if(m_tokens[m_index].lexeme == Lexeme::SEMICOLON) {
                                    return intStatement;
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
