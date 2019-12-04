/// This
#include "Parser.hpp"

/// Statements
#include "statements/ArrowStatement.hpp"
#include "statements/ArrowlessStatement.hpp"
#include "statements/CallStatement.hpp"
#include "statements/ForStatement.hpp"
#include "statements/IfStatement.hpp"
#include "statements/EchoStatement.hpp"
#include "statements/ElseStatement.hpp"
#include "statements/ElseIfStatement.hpp"
#include "statements/FunctionStatement.hpp"
#include "statements/RepeatStatement.hpp"
#include "statements/SimpleArrowStatement.hpp"
#include "statements/StartStatement.hpp"
#include "statements/StringToIntStatement.hpp"
#include "statements/WhileStatement.hpp"
#include "statements/ExitStatement.hpp"
#include "statements/AnsiStatement.hpp"
#include "statements/ArgStatement.hpp"
#include "statements/LengthStatement.hpp"
#include "statements/ReleaseStatement.hpp"
#include "statements/EraseStatement.hpp"
#include "statements/MatchesStatement.hpp"

/// Expressions
#include "expressions/BooleanExpression.hpp"
#include "expressions/BracedExpressionCollectionExpression.hpp"
#include "expressions/GroupedExpression.hpp"
#include "expressions/HatHatStringExpression.hpp"
#include "expressions/HatStringExpression.hpp"
#include "expressions/IdentifierExpression.hpp"
#include "expressions/IndexExpression.hpp"
#include "expressions/ListExpression.hpp"
#include "expressions/ListWordExpression.hpp"
#include "expressions/LiteralIntExpression.hpp"
#include "expressions/LiteralRealExpression.hpp"
#include "expressions/LiteralStringExpression.hpp"
#include "expressions/MathExpression.hpp"
#include "expressions/OperatorExpression.hpp"
#include "expressions/QQStringExpression.hpp"
#include "expressions/QStringExpression.hpp"
#include "expressions/SingleEqualExpression.hpp"
#include "expressions/DoubleEqualExpression.hpp"
#include "expressions/FunctionExpression.hpp"

/// Other
#include "LanguageException.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include "lexer/Lexeme.hpp"
#include <algorithm>
#include <functional>
#include <utility>

namespace arrow {

    std::map<std::string, std::shared_ptr<FunctionStatement>> Parser::m_functions{};

    Parser::Parser(std::vector<Token> tokens)
      : m_tokens(std::move(tokens))
      , m_current(std::begin(m_tokens))
    {
    }

    Parser::Parser()
      : m_tokens()
      , m_current()
    {
    }

    void Parser::setTokens(std::vector<Token> tokens)
    {
        m_statements.clear();
        m_tokens = std::move(tokens);
        m_current = std::begin(m_tokens);
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

    std::vector<Type> Parser::parseProgramArguments()
    {
        std::vector<Type> types;
        Cache notUsed;

        while(notAtEnd()) {
            auto expression = parseLiteralIntExpression();
            if(!expression) {
                break;
            }
            auto const evaluated = expression->getEvaluator()->evaluate(notUsed);
            types.push_back(evaluated);
            advanceTokenIterator();
        }
        return types;
    }

    std::shared_ptr<Statement> Parser::getStartStatement() const
    {
        return m_startStatement;
    }

    std::vector<std::shared_ptr<Statement>> Parser::getStatements() const
    {
        return m_statements;
    }

    std::shared_ptr<FunctionStatement> Parser::getFunction(std::string identifier)
    {
        auto found = std::find_if(std::begin(m_functions),
                                  std::end(m_functions),
                                  [identifier{std::move(identifier)}]
                                  (auto const & p) {
            return p.first == identifier;
        });

        if(found != std::end(m_functions)) {
            return found->second;
        }
        return nullptr;
    }

    std::shared_ptr<Statement> Parser::buildStatement()
    {

        // Skip any comments
        while(currentToken().lexeme == Lexeme::COMMENT) {
            advanceTokenIterator();
        }

        if(m_current + 1 != std::end(m_tokens)) {
            static std::vector<std::function<std::shared_ptr<Statement>(void)>> pvec;
            if(pvec.empty()) {
                //pvec.emplace_back([this]{return parseCallStatement();});
                pvec.emplace_back([this]{return parseFunctionStatement();});
                pvec.emplace_back([this]{return parseSimpleArrowStatement();});
                pvec.emplace_back([this]{return parseArrowStatement();});
                pvec.emplace_back([this]{return parseMatchesStatement();});
                pvec.emplace_back([this]{return parseReleaseStatement();});
                pvec.emplace_back([this]{return parseArrowlessStatement();});
                pvec.emplace_back([this]{return parseRepeatStatement();});
                pvec.emplace_back([this]{return parseWhileStatement();});
                pvec.emplace_back([this]{return parseForStatement();});
                pvec.emplace_back([this]{return parseIfStatement();});
                pvec.emplace_back([this]{return parseStartStatement();});
            }
            for(auto const & p : pvec) {
                auto store = m_current;
                auto statement = p();
                if(statement) {
                    return statement;
                }
                m_current = store;
            }
        }
        throw LanguageException("Unable to parse statement", currentToken().lineNumber);
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
        if(m_current == std::end(m_tokens)) {
            throw std::runtime_error("Parse error");
        }
        return *m_current;
    }

    Token Parser::nextToken() const
    {
        if(m_current + 1 == std::end(m_tokens)) {
            throw std::runtime_error("Parse error");
        }
        return *(m_current + 1);
    }

    std::shared_ptr<Expression> Parser::parseIdentifierExpression()
    {
        auto const ln = currentToken().lineNumber;
        auto exp = std::make_shared<IdentifierExpression>(ln);
        exp->withIdentifierToken(currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseIndexExpression()
    {
        if(nextToken().lexeme != Lexeme::OPEN_SQUARE) {
            return nullptr;
        }
        auto const ln = currentToken().lineNumber;
        auto exp = std::make_shared<IndexExpression>(ln);

        exp->withIdentifierToken(currentToken());
        advanceTokenIterator();
        advanceTokenIterator();
        auto innerExpression = parseExpression();
        exp->withIndexExpression(std::move(innerExpression));
        advanceTokenIterator();
        if(currentToken().lexeme != Lexeme::CLOSE_SQUARE) {
            return nullptr;
        }
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseListWordExpression()
    {
        auto const ln = currentToken().lineNumber;
        auto exp = std::make_shared<ListWordExpression>(ln);
        exp->withWordToken(currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseLiteralIntExpression()
    {
        auto const ln = currentToken().lineNumber;
        auto exp = std::make_shared<LiteralIntExpression>(ln);
        exp->withIntToken(currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseLiteralRealExpression()
    {
        auto const ln = currentToken().lineNumber;
        auto exp = std::make_shared<LiteralRealExpression>(ln);
        exp->withRealToken(currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseLiteralStringExpression()
    {
        auto const ln = currentToken().lineNumber;
        auto exp = std::make_shared<LiteralStringExpression>(ln);
        exp->withStringToken(currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseHatStringExpression()
    {
        auto const ln = currentToken().lineNumber;
        auto exp = std::make_shared<HatStringExpression>(ln);
        exp->withHatStringToken(currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseHatHatStringExpression()
    {
        auto const ln = currentToken().lineNumber;
        auto exp = std::make_shared<HatHatStringExpression>(ln);
        exp->withHatHatStringToken(currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseQStringExpression()
    {
        auto const ln = currentToken().lineNumber;
        auto exp = std::make_shared<QStringExpression>(ln);
        exp->withQStringToken(currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseQQStringExpression()
    {
        auto const ln = currentToken().lineNumber;
        auto exp = std::make_shared<QQStringExpression>(ln);
        exp->withQQStringToken(currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseSingleEqualExpression()
    {
        auto const ln = currentToken().lineNumber;
        auto exp = std::make_shared<SingleEqualExpression>(ln);
        exp->withToken(currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseDoubleEqualExpression()
    {
        auto const ln = currentToken().lineNumber;
        auto exp = std::make_shared<DoubleEqualExpression>(ln);
        exp->withToken(currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseOperatorExpression()
    {
        auto const ln = currentToken().lineNumber;
        auto exp = std::make_shared<OperatorExpression>(ln);
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

    std::shared_ptr<Expression> Parser::parseBooleanExpression()
    {
        auto const ln = currentToken().lineNumber;
        auto exp = std::make_shared<BooleanExpression>(ln);
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

    std::shared_ptr<Expression> Parser::parseMathExpression()
    {
        auto const ln = currentToken().lineNumber;
        auto exp = std::make_shared<MathExpression>(ln);
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
        auto const ln = currentToken().lineNumber;
        auto exp = std::make_shared<GroupedExpression>(ln);
        if(expression) {

            exp->withExpression(std::move(expression));

            // Skip to get to next paren
            advanceTokenIterator();

            if(currentToken().lexeme != Lexeme::CLOSE_PAREN) {
                return nullptr;
            }

            // Check for additional parts
            if(isMathOperator(nextToken().lexeme)) {
                auto extra = std::make_shared<MathExpression>(ln);
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
            } else if(isBooleanOperator(nextToken().lexeme)) {
                auto extra = std::make_shared<BooleanExpression>(ln);
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
        auto const ln = currentToken().lineNumber;
        auto listExp = std::make_shared<ListExpression>(ln);
        while(currentToken().lexeme != Lexeme::CLOSE_SQUARE) {
            // Note, use partListExpressionType here rather
            // than parseExpression to make it easier to
            // distinguish between plain words and identifiers
            // both of which are generic string lexeme types.
            auto exp = parseListExpressionType();
            if(!exp) { return nullptr; }
            listExp->addPart(std::move(exp));
            advanceTokenIterator();
        }
        return listExp;
    }

    std::shared_ptr<Expression> Parser::parseFunctionExpression()
    {
        if(currentToken().lexeme != Lexeme::GENERIC_STRING) { return nullptr; }
        if(nextToken().lexeme != Lexeme::OPEN_PAREN) { return nullptr; }
        auto const ln = currentToken().lineNumber;
        auto functionExpression = std::make_shared<FunctionExpression>(ln);
        functionExpression->withFunctionNameToken(currentToken());
        advanceTokenIterator();
        auto collection = parseExpressionCollectionExpression();
        if(!collection) { return nullptr; }
        functionExpression->withExpressionCollection(std::move(collection));
        return functionExpression;
    }

    std::shared_ptr<Expression> 
    Parser::parseExpressionCollectionExpression(bool const identifierOnly)
    {
        if(currentToken().lexeme != Lexeme::OPEN_PAREN) { return nullptr; }
        auto const ln = currentToken().lineNumber;
        auto expression = std::make_shared<ExpressionCollectionExpression>(ln);
        advanceTokenIterator();
        while(currentToken().lexeme != Lexeme::CLOSE_PAREN) {
            std::shared_ptr<Expression> exp;
            if(identifierOnly) {
                if(currentToken().lexeme != Lexeme::GENERIC_STRING) {
                    exp = nullptr; // error
                } else {
                    exp = parseListWordExpression();
                }
            } else {
                exp = parseExpression();
            }

            if(!exp) { 
                // When there are no parameters
                if(currentToken().lexeme == Lexeme::CLOSE_PAREN) {
                    break;
                }
                return nullptr; 
            }
            expression->addExpression(std::move(exp));
            advanceTokenIterator();
            if(currentToken().lexeme == Lexeme::COMMA) {
                advanceTokenIterator();
            }
        }
        return expression;
    }

    std::shared_ptr<Expression> 
    Parser::parseBracedExpressionCollectionExpression()
    {
        if(currentToken().lexeme != Lexeme::OPEN_CURLY) { return nullptr; }
        auto const ln = currentToken().lineNumber;
        auto expression = std::make_shared<BracedExpressionCollectionExpression>(ln);
        advanceTokenIterator();
        while(currentToken().lexeme != Lexeme::CLOSE_CURLY) {
            std::shared_ptr<Expression> exp = parseExpression();
            
            if(!exp) { 
                return nullptr; 
            }
            expression->addExpression(std::move(exp));
            advanceTokenIterator();
            if(currentToken().lexeme == Lexeme::COMMA) {
                advanceTokenIterator();
            }
        }
        return expression;
    }

    std::shared_ptr<Expression> Parser::parseListExpressionType()
    {
        if(currentToken().lexeme == Lexeme::GENERIC_STRING) {
            return parseListWordExpression();
        } else if(currentToken().lexeme == Lexeme::HAT_HAT_STRING) {
            return parseHatHatStringExpression();
        } else if(currentToken().lexeme == Lexeme::HAT_STRING) {
            return parseHatStringExpression();
        } else if(currentToken().lexeme == Lexeme::Q_Q_STRING) {
            return parseQQStringExpression();
        } else if(currentToken().lexeme == Lexeme::Q_STRING) {
            return parseQStringExpression();
        } else if(currentToken().lexeme == Lexeme::OPEN_SQUARE) {
            return parseListExpression();
        } else if(currentToken().lexeme == Lexeme::EQUAL) {
            return parseSingleEqualExpression();
        } else if(currentToken().lexeme == Lexeme::EQUAL_EQUAL) {
            return parseDoubleEqualExpression();
        } else if(currentToken().lexeme == Lexeme::INTEGER_NUM) {
            return parseLiteralIntExpression();
        } else if(currentToken().lexeme == Lexeme::REAL_NUM) {
            return parseLiteralRealExpression();
        } 
        return nullptr;
    }

    std::shared_ptr<Expression> Parser::parseExpression(bool checkOperator)
    {
        if(m_current + 1 != std::end(m_tokens)) {
            if (checkOperator && isBooleanOperator(nextToken().lexeme) &&
                currentToken().lexeme != Lexeme::OPEN_SQUARE) {
                return parseBooleanExpression();
            } else if(checkOperator && isMathOperator(nextToken().lexeme))  {
                return parseMathExpression();
            } else if(currentToken().lexeme == Lexeme::OPEN_PAREN) {

                auto store = m_current;
                auto exp = parseGroupedExpression();
                if(!exp) {
                    m_current = store;
                    // try (a, b, c) collection
                    return parseExpressionCollectionExpression();
                }
                return exp;
            } else if(currentToken().lexeme == Lexeme::OPEN_CURLY) {
                return parseBracedExpressionCollectionExpression();
            } else if(currentToken().lexeme == Lexeme::OPEN_SQUARE) {
                return parseListExpression();
            } else if(currentToken().lexeme == Lexeme::INTEGER_NUM) {
                return parseLiteralIntExpression();
            } else if(currentToken().lexeme == Lexeme::REAL_NUM) {
                return parseLiteralRealExpression();
            } else if(currentToken().lexeme == Lexeme::GENERIC_STRING) {
                auto store = m_current;
                auto exp = parseFunctionExpression();
                if(!exp) {
                    m_current = store;
                    store = m_current;
                    exp = parseIndexExpression();
                    if(!exp) {
                        m_current = store;
                        return parseIdentifierExpression();
                    }
                }
                return exp;
            } else if(currentToken().lexeme == Lexeme::LITERAL_STRING) {
                return parseLiteralStringExpression();
            }
        }
        return nullptr;
    }

    std::shared_ptr<Statement> Parser::parseArrowStatement()
    {
        auto const ln = currentToken().lineNumber;
        // Don't attempt to parse function statements
        if(currentToken().raw == "fn") {
            return nullptr;
        }
        auto arrowStatement = std::make_shared<ArrowStatement>(ln);
        arrowStatement->withToken(currentToken());
        auto const keyword = currentToken().raw;
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
                                if(keyword == "stoi") {
                                    return std::make_shared<StringToIntStatement>(ln, *arrowStatement);
                                } else if(keyword == "arg") {
                                    return std::make_shared<ArgStatement>(ln, *arrowStatement);
                                } else if(keyword == "length") {
                                    return std::make_shared<LengthStatement>(ln, *arrowStatement);
                                }
                                return arrowStatement;
                            }
                        }
                    }
                }
            }
        }
        return nullptr;
    }

    std::shared_ptr<Statement> Parser::parseSimpleArrowStatement()
    {
        auto const ln = currentToken().lineNumber;
        auto arrowStatement = std::make_shared<SimpleArrowStatement>(ln);
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
                    if(currentToken().lexeme == Lexeme::GENERIC_STRING ||
                       currentToken().lexeme == Lexeme::DOLLAR_STRING) {
                        arrowStatement->withIdentifier(currentToken());
                        if(nextToken().lexeme == Lexeme::OPEN_SQUARE) {
                            auto indexExp = parseIndexExpression();
                            arrowStatement->withIndexExpression(std::move(indexExp));
                        } 
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
        auto const ln = currentToken().lineNumber;
        auto arrowlessStatement = std::make_shared<ArrowlessStatement>(ln);
        arrowlessStatement->withToken(currentToken());
        advanceTokenIterator();
        auto const keyword = arrowlessStatement->getToken().raw;
        if(keyword == "erase") {
            auto expression = parseIndexExpression();
            if(!expression) {
                throw LanguageException("Not an index expression", ln);
            }
            arrowlessStatement->withExpression(std::move(expression));
            advanceTokenIterator();
            if(currentToken().lexeme == Lexeme::SEMICOLON) {
                return std::make_shared<EraseStatement>(ln, std::move(arrowlessStatement));
            }
        } else {
            auto expression = parseExpression();
            if(expression) {
                arrowlessStatement->withExpression(std::move(expression));
                advanceTokenIterator();
                if(notAtEnd()) {
                    if(currentToken().lexeme == Lexeme::SEMICOLON) {
                        
                        if(keyword == "prn" || keyword == "pr" || keyword == "say") {
                            return std::make_shared<EchoStatement>(ln, std::move(arrowlessStatement));
                        } else if(keyword == "exit") {
                            return std::make_shared<ExitStatement>(ln, std::move(arrowlessStatement));
                        } else if(keyword == "ansi_up") {
                            return std::make_shared<AnsiStatement>(ln, std::move(arrowlessStatement));
                        }
                        return arrowlessStatement;
                    }
                }
            }
        }
        return nullptr;
    }

    std::shared_ptr<Statement> Parser::parseReleaseStatement()
    {
        if(currentToken().raw != "release") {
            return nullptr;
        }
        auto const ln = currentToken().lineNumber;
        auto releaseStatement = std::make_shared<ReleaseStatement>(ln);
        releaseStatement->withToken(currentToken());
        advanceTokenIterator();
        releaseStatement->withIdentifier(currentToken());
        advanceTokenIterator();
        if(currentToken().lexeme == Lexeme::SEMICOLON) {
            return releaseStatement;
        }
        return nullptr;
    }

    std::shared_ptr<Statement> Parser::parseRepeatStatement()
    {
        if(currentToken().raw != "repeat") { return nullptr; }
        auto const ln = currentToken().lineNumber;
        auto repeatStatement = std::make_shared<RepeatStatement>(ln);
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
        auto const ln = currentToken().lineNumber;
        auto whileStatement = std::make_shared<WhileStatement>(ln);
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
        auto const ln = currentToken().lineNumber;
        auto forStatement = std::make_shared<ForStatement>(ln);
        forStatement->withToken(currentToken());
        advanceTokenIterator();
        if(currentToken().lexeme != Lexeme::GENERIC_STRING) { return nullptr; }
        forStatement->withIndexer(currentToken());
        advanceTokenIterator();
        if(currentToken().raw != "in") { return nullptr; }
        advanceTokenIterator();
        if(currentToken().lexeme != Lexeme::GENERIC_STRING) { return nullptr; }
        forStatement->withIdentifier(currentToken());
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
        auto ln = currentToken().lineNumber;
        auto ifStatement = std::make_shared<IfStatement>(ln);
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
            ln = currentToken().lineNumber;
            auto elseIfStatement = std::make_shared<ElseIfStatement>(ln);
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
            ln = currentToken().lineNumber;
            advanceTokenIterator();
            auto elseStatement = std::make_shared<ElseStatement>(ln);
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
        if(currentToken().lexeme != Lexeme::GENERIC_STRING) { return nullptr; }
        auto const ln = currentToken().lineNumber;
        auto callStatement = std::make_shared<CallStatement>(ln);
        callStatement->withFunctionNameToken(currentToken());
        advanceTokenIterator();
        auto collection = parseExpressionCollectionExpression();
        if(!collection) { return nullptr; }
        callStatement->withExpressionCollection(std::move(collection));
        advanceTokenIterator();

        // Handle 'arrow-less' condition (not returning call).
        if(currentToken().lexeme != Lexeme::ARROW) {
            if(currentToken().lexeme == Lexeme::SEMICOLON) {
                callStatement->withIdentifier(Token{Lexeme::NIL, "nil", currentToken().lineNumber});
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
        auto const ln = currentToken().lineNumber;
        auto startStatement = std::make_shared<StartStatement>(ln);
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
        m_startStatement = startStatement;
        return startStatement;
    }

    std::shared_ptr<Statement> Parser::parseFunctionStatement()
    {
        if(currentToken().raw != "fn") { return nullptr; }
        auto const ln = currentToken().lineNumber;
        auto functionStatement = std::make_shared<FunctionStatement>(ln);
        functionStatement->withToken(currentToken());
        advanceTokenIterator();
        if(currentToken().lexeme != Lexeme::GENERIC_STRING) { return nullptr; }
        auto const theName = currentToken();
        functionStatement->withNameToken(theName);
        advanceTokenIterator();
        auto collection = parseExpressionCollectionExpression(true /* ident only */);
        if(!collection) { return nullptr; }
        functionStatement->withExpressionCollection(std::move(collection));
        advanceTokenIterator();
        // Handle 'arrow-less' condition (not returning call).
        if(currentToken().lexeme != Lexeme::ARROW) {
            functionStatement->withReturnIdentifierToken(Token{Lexeme::NIL, "nil", currentToken().lineNumber});
        } else {
            advanceTokenIterator();
            if(currentToken().lexeme != Lexeme::GENERIC_STRING) { return nullptr; }
            functionStatement->withReturnIdentifierToken(currentToken());
            advanceTokenIterator();
        }
        if(currentToken().lexeme != Lexeme::OPEN_CURLY) { return nullptr; }
        advanceTokenIterator();
        while(currentToken().lexeme != Lexeme::CLOSE_CURLY) {
            auto statement = buildStatement();
            if(statement) {
                functionStatement->addBodyStatement(std::move(statement));
            }
            advanceTokenIterator();
        }

        m_functions.emplace(theName.raw, functionStatement);
        return functionStatement;
    }

    std::shared_ptr<Statement> Parser::parseMatchesStatement()
    {
        auto const ln = currentToken().lineNumber;
        auto matchesStatement = std::make_shared<MatchesStatement>(ln);
        auto const expLeft = parseExpression();
        if(!expLeft) {
            return nullptr;
        }
        matchesStatement->withLeftExpression(expLeft);
        advanceTokenIterator();
        auto const keyword = currentToken();
        if(keyword.raw != "matches") {
            return nullptr;
        }
        advanceTokenIterator();
        auto const expRight = parseExpression();
        if(!expRight) {
            return nullptr;
        }
        matchesStatement->withRightExpression(expRight);
        advanceTokenIterator();
        if(currentToken().lexeme != Lexeme::ARROW) {
            return nullptr;
        }
        advanceTokenIterator();
        if(currentToken().lexeme != Lexeme::GENERIC_STRING) {
            return nullptr;
        }
        matchesStatement->withIdentifier(currentToken());
        advanceTokenIterator();
        if(currentToken().lexeme != Lexeme::SEMICOLON) {
            return nullptr;
        }
        return matchesStatement;
    }
}
