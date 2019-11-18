/// This
#include "Parser.hpp"

/// Statements
#include "ArrowStatement.hpp"
#include "ArrowlessStatement.hpp"
#include "CallStatement.hpp"
#include "ForStatement.hpp"
#include "IfStatement.hpp"
#include "ElseStatement.hpp"
#include "ElseIfStatement.hpp"
#include "RepeatStatement.hpp"
#include "StartStatement.hpp"
#include "WhileStatement.hpp"

/// Expressions
#include "GroupedExpression.hpp"
#include "HatHatStringExpression.hpp"
#include "HatStringExpression.hpp"
#include "IdentifierExpression.hpp"
#include "ListExpression.hpp"
#include "LiteralIntExpression.hpp"
#include "LiteralRealExpression.hpp"
#include "LiteralStringExpression.hpp"
#include "OperatorExpression.hpp"
#include "QQStringExpression.hpp"
#include "QStringExpression.hpp"

/// Other
#include "Lexeme.hpp"
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
            auto statement = buildStatement();
            if(!statement) {
                break; // error
            }
            m_statements.emplace_back(std::move(statement));
            advanceTokenIterator();
        }
    }

    std::vector<std::shared_ptr<Statement>> Parser::getStatements() const
    {
        return m_statements;
    }

    std::shared_ptr<Statement> Parser::buildStatement()
    {
        if(currentToken().lexeme == Lexeme::GENERIC_STRING) {
            {
                auto store = m_current;
                auto statement = parseArrowStatement();
                if(statement) { 
                    return statement;
                }
                // Revert token iterator state in case of failure
                m_current = store;
            }
            {
                auto store = m_current;
                auto statement = parseArrowlessStatement();
                if(statement) { 
                    return statement;
                }
                // Revert token iterator state in case of failure
                m_current = store;
            }
            {
                auto store = m_current;
                auto statement = parseRepeatStatement();
                if(statement) { 
                    return statement;
                }
                // Revert token iterator state in case of failure
                m_current = store;
            }
            {
                auto store = m_current;
                auto statement = parseWhileStatement();
                if(statement) { 
                    return statement;
                }
                // Revert token iterator state in case of failure
                m_current = store;
            }
            {
                auto store = m_current;
                auto statement = parseForStatement();
                if(statement) { 
                    return statement;
                }
                // Revert token iterator state in case of failure
                m_current = store;
            }
            {
                auto store = m_current;
                auto statement = parseIfStatement();
                if(statement) { 
                    return statement;
                }
                // Revert token iterator state in case of failure
                m_current = store;
            }
            {
                auto store = m_current;
                auto statement = parseCallStatement();
                if(statement) { 
                    return statement;
                }
                // Revert token iterator state in case of failure
                m_current = store;
            }
            {
                auto store = m_current;
                auto statement = parseStartStatement();
                if(statement) { 
                    return statement;
                }
                // Revert token iterator state in case of failure
                m_current = store;
            }
        }
        return nullptr;
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

    std::shared_ptr<Expression> Parser::parseExpressionCollectionExpression()
    {
        if(currentToken().lexeme != Lexeme::OPEN_PAREN) { return nullptr; }
        auto expression = std::make_shared<ExpressionCollectionExpression>();
        advanceTokenIterator();

        while(nextToken().lexeme == Lexeme::COMMA) {
            auto exp = parseExpression();
            if(!exp) { return nullptr; }
            expression->addExpression(std::move(exp));
            advanceTokenIterator();
            advanceTokenIterator();
        }
        auto exp = parseExpression();
        if(!exp) { return nullptr; }

        expression->addExpression(std::move(exp));
        advanceTokenIterator();
        if(currentToken().lexeme != Lexeme::CLOSE_PAREN) {
            return nullptr;
        }
        return expression;
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

    std::shared_ptr<Statement> Parser::parseRepeatStatement()
    {
        if(currentToken().raw != "repeat") { return nullptr; }
        auto repeatStatement = std::make_shared<RepeatStatement>();
        repeatStatement->withToken(currentToken());
        advanceTokenIterator();
        auto expression = parseExpression();
        if(expression) {
            repeatStatement->withExpression(std::move(expression));
            advanceTokenIterator();
            if(currentToken().raw != "times") { return nullptr; }
            advanceTokenIterator();
            if(currentToken().lexeme != Lexeme::OPEN_CURLY) { return nullptr; }
            advanceTokenIterator();
            while(currentToken().lexeme != Lexeme::CLOSE_CURLY) {
                auto statement = buildStatement();
                if(statement) {
                    repeatStatement->addBodyStatement(std::move(statement));
                }
                advanceTokenIterator();
            }
        }
        return repeatStatement;
    }

    std::shared_ptr<Statement> Parser::parseWhileStatement()
    {
        if(currentToken().raw != "while") { return nullptr; }
        auto whileStatement = std::make_shared<WhileStatement>();
        whileStatement->withToken(currentToken());
        advanceTokenIterator();
        if(currentToken().lexeme != Lexeme::OPEN_PAREN) { return nullptr; }
        auto expression = parseGroupedExpression();
        if(expression) {
            whileStatement->withExpression(std::move(expression));
            advanceTokenIterator();
            if(currentToken().lexeme != Lexeme::OPEN_CURLY) { return nullptr; }
            advanceTokenIterator();
            while(currentToken().lexeme != Lexeme::CLOSE_CURLY) {
                auto statement = buildStatement();
                if(statement) {
                    whileStatement->addBodyStatement(std::move(statement));
                }
                advanceTokenIterator();
            }
        }
        return whileStatement;
    }

    std::shared_ptr<Statement> Parser::parseForStatement()
    {
        if(currentToken().raw != "for") { return nullptr; }
        auto forStatement = std::make_shared<ForStatement>();
        forStatement->withToken(currentToken());
        advanceTokenIterator();
        if(currentToken().lexeme != Lexeme::GENERIC_STRING) { return nullptr; }
        forStatement->withIdentifierA(currentToken());
        advanceTokenIterator();
        if(currentToken().raw != "in") { return nullptr; }
        advanceTokenIterator();
        if(currentToken().lexeme != Lexeme::GENERIC_STRING) { return nullptr; }
        forStatement->withIdentifierB(currentToken());
        advanceTokenIterator();
        if(currentToken().lexeme != Lexeme::OPEN_CURLY) { return nullptr; }
        advanceTokenIterator();
        while(currentToken().lexeme != Lexeme::CLOSE_CURLY) {
            auto statement = buildStatement();
            if(statement) {
                forStatement->addBodyStatement(std::move(statement));
            }
            advanceTokenIterator();
        }
        return forStatement;
     }

    std::shared_ptr<Statement> Parser::parseIfStatement()
    {
        if(currentToken().raw != "if") { return nullptr; }
        auto ifStatement = std::make_shared<IfStatement>();
        ifStatement->withToken(currentToken());
        if(nextToken().lexeme != Lexeme::OPEN_PAREN) { return nullptr; }
        advanceTokenIterator();
        {
            auto expression = parseGroupedExpression();
            if(!expression) { return nullptr; }
            ifStatement->withExpression(expression);
        }
        advanceTokenIterator();
        if(currentToken().lexeme != Lexeme::OPEN_CURLY) { return nullptr; }
        advanceTokenIterator();
        while(currentToken().lexeme != Lexeme::CLOSE_CURLY) {
            auto statement = buildStatement();
            if(statement) {
                ifStatement->addBodyStatement(std::move(statement));
            }
            advanceTokenIterator();
        }
        while(nextToken().raw == "elseif") {
            advanceTokenIterator();
            auto elseIfStatement = std::make_shared<ElseIfStatement>();
            elseIfStatement->withToken(currentToken());
            if(nextToken().lexeme != Lexeme::OPEN_PAREN) { return nullptr; }
            advanceTokenIterator();
            {
                auto expression = parseGroupedExpression();
                if(!expression) { return nullptr; }
                elseIfStatement->withExpression(expression);
            }
            advanceTokenIterator();
            if(currentToken().lexeme != Lexeme::OPEN_CURLY) { return nullptr; }
            advanceTokenIterator();
            while(currentToken().lexeme != Lexeme::CLOSE_CURLY) {
                auto statement = buildStatement();
                if(statement) {
                    elseIfStatement->addBodyStatement(std::move(statement));
                }
                advanceTokenIterator();
            }
            ifStatement->addElseIfPart(std::move(elseIfStatement));
        }
        if(nextToken().raw == "else") {
            advanceTokenIterator();
            auto elseStatement = std::make_shared<ElseStatement>();
            elseStatement->withToken(currentToken());
            advanceTokenIterator();
            if(currentToken().lexeme != Lexeme::OPEN_CURLY) { return nullptr; }
            advanceTokenIterator();
            while(currentToken().lexeme != Lexeme::CLOSE_CURLY) {
                auto statement = buildStatement();
                if(statement) {
                    elseStatement->addBodyStatement(std::move(statement));
                }
                advanceTokenIterator();
            }
            ifStatement->withElsePart(std::move(elseStatement));
        }
        return ifStatement;
    }

    std::shared_ptr<Statement> Parser::parseCallStatement()
    {
        if(currentToken().raw != "call") { return nullptr; }
        auto callStatement = std::make_shared<CallStatement>();
        callStatement->withToken(currentToken());
        advanceTokenIterator();
        if(currentToken().lexeme != Lexeme::GENERIC_STRING) { return nullptr; }
        callStatement->withFunctionNameToken(currentToken());
        advanceTokenIterator();
        auto collection = parseExpressionCollectionExpression();
        if(!collection) { return nullptr; }
        callStatement->withExpressionCollection(std::move(collection));
        advanceTokenIterator();

        // Handle 'arrow-less' condition (not returning call).
        if(currentToken().lexeme != Lexeme::ARROW) {
            if(currentToken().lexeme == Lexeme::SEMICOLON) {
                return callStatement;
            }
            return nullptr;
        }
        advanceTokenIterator();
        if(currentToken().lexeme != Lexeme::GENERIC_STRING) { return nullptr; }
        callStatement->withIdentifier(currentToken());
        advanceTokenIterator();
        if(currentToken().lexeme != Lexeme::SEMICOLON) { return nullptr; }
        return callStatement;
    }

    std::shared_ptr<Statement> Parser::parseStartStatement()
    {
        if(currentToken().raw != "start") { return nullptr; }
        auto startStatement = std::make_shared<StartStatement>();
        startStatement->withToken(currentToken());
        advanceTokenIterator();
        if(currentToken().lexeme != Lexeme::OPEN_CURLY) { return nullptr; }
        advanceTokenIterator();
        while(currentToken().lexeme != Lexeme::CLOSE_CURLY) {
            auto statement = buildStatement();
            if(statement) {
                startStatement->addBodyStatement(std::move(statement));
            }
            advanceTokenIterator();
        }
        return startStatement;
    }
}
