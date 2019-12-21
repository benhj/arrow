/// (c) Ben Jones 2019 - present

/// This
#include "Parser.hpp"

/// Statements
#include "statements/AnsiStatement.hpp"
#include "statements/ArgStatement.hpp"
#include "statements/ArrowlessStatement.hpp"
#include "statements/ArrowStatement.hpp"
#include "statements/AsyncStatement.hpp"
#include "statements/EchoStatement.hpp"
#include "statements/ElseIfStatement.hpp"
#include "statements/ElseStatement.hpp"
#include "statements/EraseStatement.hpp"
#include "statements/ExitStatement.hpp"
#include "statements/ForStatement.hpp"
#include "statements/FunctionStatement.hpp"
#include "statements/IfStatement.hpp"
#include "statements/LengthStatement.hpp"
#include "statements/LoopBreakStatement.hpp"
#include "statements/ReleaseStatement.hpp"
#include "statements/RepeatStatement.hpp"
#include "statements/ReturnStatement.hpp"
#include "statements/SimpleArrowStatement.hpp"
#include "statements/SingleExpressionStatement.hpp"
#include "statements/StartStatement.hpp"
#include "statements/StringToIntStatement.hpp"
#include "statements/WhileStatement.hpp"

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
#include "evaluator/ExpressionEvaluator.hpp"
#include "evaluator/StatementEvaluator.hpp"
#include "LanguageException.hpp"
#include "lexer/Lexeme.hpp"
#include <algorithm>
#include <functional>
#include <utility>

namespace arrow {

    std::map<std::string, std::shared_ptr<FunctionStatement>> Parser::m_functions{};

    Parser::Parser(std::vector<Token> tokens)
      : m_tm(std::move(tokens))
    {
    }

    void Parser::parse()
    {
        while(m_tm.notAtEnd()) {
            auto statement = buildStatement();
            if(!statement) {
                break; // error
            }
            m_statements.emplace_back(std::move(statement));
            m_tm.advanceTokenIterator();
        }
    }

    std::vector<Type> Parser::parseProgramArguments()
    {
        std::vector<Type> types;
        Cache notUsed;

        while(m_tm.notAtEnd()) {
            auto expression = parseLiteralIntExpression();
            if(!expression) {
                break;
            }
            auto const evaluated = expression->getEvaluator()->evaluate(notUsed);
            types.push_back(evaluated);
            m_tm.advanceTokenIterator();
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
        while(m_tm.currentToken().lexeme == Lexeme::COMMENT) {
            m_tm.advanceTokenIterator();
        }

        if(m_tm.tokenPlusOneNotAtEnd()) {

            static std::vector<std::function<std::shared_ptr<Statement>(void)>> pvec;
            if(pvec.empty()) {
                pvec.emplace_back([this]{return parseBreakStatement();});
                pvec.emplace_back([this]{return parseReturnStatement();});
                pvec.emplace_back([this]{return parseFunctionStatement();});
                pvec.emplace_back([this]{return parseSimpleArrowStatement();});
                pvec.emplace_back([this]{return parseArrowStatement();});
                pvec.emplace_back([this]{return parseReleaseStatement();});
                pvec.emplace_back([this]{return parseSingleExpressionStatement();});
                pvec.emplace_back([this]{return parseArrowlessStatement();});
                pvec.emplace_back([this]{return parseRepeatStatement();});
                pvec.emplace_back([this]{return parseWhileStatement();});
                pvec.emplace_back([this]{return parseForStatement();});
                pvec.emplace_back([this]{return parseIfStatement();});
                pvec.emplace_back([this]{return parseStartStatement();});
                pvec.emplace_back([this]{return parseAsyncStatement();});
            }
            for(auto const & p : pvec) {
                auto store = m_tm.retrieveIt();
                auto statement = p();
                if(statement) {
                    return statement;
                }
                m_tm.revert(store);
            }
        }
        throw LanguageException("Unable to parse statement", m_tm.currentToken().lineNumber);
    }

    std::shared_ptr<Expression> Parser::parseIdentifierExpression()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<IdentifierExpression>(ln);
        exp->withIdentifierToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseIndexExpression()
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

    std::shared_ptr<Expression> Parser::parseListWordExpression()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<ListWordExpression>(ln);
        exp->withWordToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseLiteralIntExpression()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<LiteralIntExpression>(ln);
        exp->withIntToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseLiteralRealExpression()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<LiteralRealExpression>(ln);
        exp->withRealToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseLiteralStringExpression()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<LiteralStringExpression>(ln);
        exp->withStringToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseLiteralCharExpression()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<LiteralCharExpression>(ln);
        exp->withStringToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseHatStringExpression()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<HatStringExpression>(ln);
        exp->withHatStringToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseHatHatStringExpression()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<HatHatStringExpression>(ln);
        exp->withHatHatStringToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseQStringExpression()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<QStringExpression>(ln);
        exp->withQStringToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseQQStringExpression()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<QQStringExpression>(ln);
        exp->withQQStringToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseSingleEqualExpression()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<SingleEqualExpression>(ln);
        exp->withToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseDoubleEqualExpression()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<DoubleEqualExpression>(ln);
        exp->withToken(m_tm.currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseMatchesExpression()
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

    std::shared_ptr<Expression> Parser::parseOperatorExpression()
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

    std::shared_ptr<Expression> Parser::parseBooleanExpression()
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

    std::shared_ptr<Expression> Parser::parseMathExpression()
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

    std::shared_ptr<Expression> Parser::parseGroupedExpression()
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

    std::shared_ptr<Expression> Parser::parseListExpression()
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

    std::shared_ptr<Expression> Parser::parseFunctionExpression()
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
    Parser::parseExpressionCollectionExpression(bool const identifierOnly)
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
    Parser::parseBracedExpressionCollectionExpression()
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

    std::shared_ptr<Expression> Parser::parseListExpressionType()
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

    std::shared_ptr<Expression> Parser::parseExpression(bool checkOperator)
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

    std::shared_ptr<Statement> Parser::parseArrowStatement()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        // Don't attempt to parse function statements
        if(m_tm.currentToken().raw == "fn") {
            return nullptr;
        }
        auto arrowStatement = std::make_shared<ArrowStatement>(ln);
        arrowStatement->withToken(m_tm.currentToken());
        auto const keyword = m_tm.currentToken().raw;
        m_tm.advanceTokenIterator();
        
        auto expression = parseExpression();
        if(expression) {
            arrowStatement->withExpression(std::move(expression));
            m_tm.advanceTokenIterator();
            if(m_tm.notAtEnd()) {
                if(m_tm.currentToken().lexeme != Lexeme::ARROW) {
                    return nullptr;
                }
                m_tm.advanceTokenIterator();
                if(m_tm.notAtEnd()) {
                    if(m_tm.currentToken().lexeme == Lexeme::GENERIC_STRING) {
                        arrowStatement->withIdentifier(m_tm.currentToken());
                        m_tm.advanceTokenIterator();
                        if(m_tm.notAtEnd()) {
                            if(m_tm.currentToken().lexeme == Lexeme::SEMICOLON) {
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
        auto const ln = m_tm.currentToken().lineNumber;
        auto arrowStatement = std::make_shared<SimpleArrowStatement>(ln);
        auto expression = parseExpression();
        if(expression) {
            arrowStatement->withExpression(std::move(expression));
            m_tm.advanceTokenIterator();
            if(m_tm.notAtEnd()) {
                if(m_tm.currentToken().lexeme != Lexeme::ARROW) {
                    return nullptr;
                }
                m_tm.advanceTokenIterator();
                if(m_tm.notAtEnd()) {
                    if(m_tm.currentToken().lexeme == Lexeme::GENERIC_STRING ||
                       m_tm.currentToken().lexeme == Lexeme::DOLLAR_STRING) {
                        arrowStatement->withIdentifier(m_tm.currentToken());
                        if(m_tm.nextToken().lexeme == Lexeme::OPEN_SQUARE) {
                            auto indexExp = parseIndexExpression();
                            arrowStatement->withIndexExpression(std::move(indexExp));
                        } 
                        m_tm.advanceTokenIterator();
                        if(m_tm.notAtEnd()) {
                            if(m_tm.currentToken().lexeme == Lexeme::SEMICOLON) {
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
        auto const ln = m_tm.currentToken().lineNumber;
        auto arrowlessStatement = std::make_shared<ArrowlessStatement>(ln);
        arrowlessStatement->withToken(m_tm.currentToken());
        m_tm.advanceTokenIterator();
        auto const keyword = arrowlessStatement->getToken().raw;
        if(keyword == "erase") {
            auto expression = parseIndexExpression();
            if(!expression) {
                throw LanguageException("Not an index expression", ln);
            }
            arrowlessStatement->withExpression(std::move(expression));
            m_tm.advanceTokenIterator();
            if(m_tm.currentToken().lexeme == Lexeme::SEMICOLON) {
                return std::make_shared<EraseStatement>(ln, std::move(arrowlessStatement));
            }
        } else {
            auto expression = parseExpression();
            if(expression) {
                arrowlessStatement->withExpression(std::move(expression));
                m_tm.advanceTokenIterator();
                if(m_tm.notAtEnd()) {
                    if(m_tm.currentToken().lexeme == Lexeme::SEMICOLON) {
                        
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
        if(m_tm.currentToken().raw != "release") {
            return nullptr;
        }
        auto const ln = m_tm.currentToken().lineNumber;
        auto releaseStatement = std::make_shared<ReleaseStatement>(ln);
        releaseStatement->withToken(m_tm.currentToken());
        m_tm.advanceTokenIterator();
        releaseStatement->withIdentifier(m_tm.currentToken());
        m_tm.advanceTokenIterator();
        if(m_tm.currentToken().lexeme == Lexeme::SEMICOLON) {
            return releaseStatement;
        }
        return nullptr;
    }

    std::shared_ptr<Statement> Parser::parseBreakStatement()
    {
        if(m_tm.currentToken().raw != "break") { return nullptr; }
        auto const ln = m_tm.currentToken().lineNumber;
        auto breakStatement = std::make_shared<LoopBreakStatement>(ln);
        m_tm.advanceTokenIterator();
        if(m_tm.currentToken().lexeme == Lexeme::SEMICOLON) {
            return breakStatement;
        }
        return nullptr;
    }

    std::shared_ptr<Statement> Parser::parseReturnStatement()
    {
        if(m_tm.currentToken().raw != "return") { return nullptr; }
        auto const ln = m_tm.currentToken().lineNumber;
        auto returnStatement = std::make_shared<ReturnStatement>(ln);
        m_tm.advanceTokenIterator();
        if(m_tm.currentToken().lexeme == Lexeme::SEMICOLON) {
            return returnStatement;
        }
        return nullptr;
    }

    std::shared_ptr<Statement> Parser::parseRepeatStatement()
    {
        if(m_tm.currentToken().raw != "repeat") { return nullptr; }
        auto const ln = m_tm.currentToken().lineNumber;
        auto repeatStatement = std::make_shared<RepeatStatement>(ln);
        repeatStatement->withToken(m_tm.currentToken());
        m_tm.advanceTokenIterator();
        auto expression = parseExpression();
        if(expression) {
            repeatStatement->withExpression(std::move(expression));
            m_tm.advanceTokenIterator();
            if(m_tm.currentToken().raw != "times") { return nullptr; }
            m_tm.advanceTokenIterator();
            if(m_tm.currentToken().lexeme != Lexeme::OPEN_CURLY) { return nullptr; }
            m_tm.advanceTokenIterator();
            while(m_tm.currentToken().lexeme != Lexeme::CLOSE_CURLY) {
                auto statement = buildStatement();
                if(statement) {
                    repeatStatement->addBodyStatement(std::move(statement));
                }
                m_tm.advanceTokenIterator();
            }
        }
        return repeatStatement;
    }

    std::shared_ptr<Statement> Parser::parseWhileStatement()
    {
        if(m_tm.currentToken().raw != "while") { return nullptr; }
        auto const ln = m_tm.currentToken().lineNumber;
        auto whileStatement = std::make_shared<WhileStatement>(ln);
        whileStatement->withToken(m_tm.currentToken());
        m_tm.advanceTokenIterator();
        if(m_tm.currentToken().lexeme != Lexeme::OPEN_PAREN) { return nullptr; }
        auto expression = parseGroupedExpression();
        if(expression) {
            whileStatement->withExpression(std::move(expression));
            m_tm.advanceTokenIterator();
            if(m_tm.currentToken().lexeme != Lexeme::OPEN_CURLY) { return nullptr; }
            m_tm.advanceTokenIterator();
            while(m_tm.currentToken().lexeme != Lexeme::CLOSE_CURLY) {
                auto statement = buildStatement();
                if(statement) {
                    whileStatement->addBodyStatement(std::move(statement));
                }
                m_tm.advanceTokenIterator();
            }
        }
        return whileStatement;
    }

    std::shared_ptr<Statement> Parser::parseForStatement()
    {
        if(m_tm.currentToken().raw != "for") { return nullptr; }
        auto const ln = m_tm.currentToken().lineNumber;
        auto forStatement = std::make_shared<ForStatement>(ln);
        forStatement->withToken(m_tm.currentToken());
        m_tm.advanceTokenIterator();
        if(m_tm.currentToken().lexeme != Lexeme::GENERIC_STRING) { return nullptr; }
        forStatement->withIndexer(m_tm.currentToken());
        m_tm.advanceTokenIterator();
        if(m_tm.currentToken().raw != "in") { return nullptr; }
        m_tm.advanceTokenIterator();
        if(m_tm.currentToken().lexeme != Lexeme::GENERIC_STRING) { return nullptr; }
        forStatement->withIdentifier(m_tm.currentToken());
        m_tm.advanceTokenIterator();
        if(m_tm.currentToken().lexeme != Lexeme::OPEN_CURLY) { return nullptr; }
        m_tm.advanceTokenIterator();
        while(m_tm.currentToken().lexeme != Lexeme::CLOSE_CURLY) {
            auto statement = buildStatement();
            if(statement) {
                forStatement->addBodyStatement(std::move(statement));
            }
            m_tm.advanceTokenIterator();
        }
        return forStatement;
     }

    std::shared_ptr<Statement> Parser::parseIfStatement()
    {
        if(m_tm.currentToken().raw != "if") { return nullptr; }
        auto ln = m_tm.currentToken().lineNumber;
        auto ifStatement = std::make_shared<IfStatement>(ln);
        ifStatement->withToken(m_tm.currentToken());
        if(m_tm.nextToken().lexeme != Lexeme::OPEN_PAREN) { return nullptr; }
        m_tm.advanceTokenIterator();
        {
            auto expression = parseGroupedExpression();
            if(!expression) { return nullptr; }
            ifStatement->withExpression(expression);
        }
        m_tm.advanceTokenIterator();
        if(m_tm.currentToken().lexeme != Lexeme::OPEN_CURLY) { return nullptr; }
        m_tm.advanceTokenIterator();
        while(m_tm.currentToken().lexeme != Lexeme::CLOSE_CURLY) {
            auto statement = buildStatement();
            if(statement) {
                ifStatement->addBodyStatement(std::move(statement));
            }
            m_tm.advanceTokenIterator();
        }
        while(m_tm.nextToken().raw == "elseif") {
            m_tm.advanceTokenIterator();
            ln = m_tm.currentToken().lineNumber;
            auto elseIfStatement = std::make_shared<ElseIfStatement>(ln);
            elseIfStatement->withToken(m_tm.currentToken());
            if(m_tm.nextToken().lexeme != Lexeme::OPEN_PAREN) { return nullptr; }
            m_tm.advanceTokenIterator();
            {
                auto expression = parseGroupedExpression();
                if(!expression) { return nullptr; }
                elseIfStatement->withExpression(expression);
            }
            m_tm.advanceTokenIterator();
            if(m_tm.currentToken().lexeme != Lexeme::OPEN_CURLY) { return nullptr; }
            m_tm.advanceTokenIterator();
            while(m_tm.currentToken().lexeme != Lexeme::CLOSE_CURLY) {
                auto statement = buildStatement();
                if(statement) {
                    elseIfStatement->addBodyStatement(std::move(statement));
                }
                m_tm.advanceTokenIterator();
            }
            ifStatement->addElseIfPart(std::move(elseIfStatement));
        }
        if(m_tm.nextToken().raw == "else") {
            ln = m_tm.currentToken().lineNumber;
            m_tm.advanceTokenIterator();
            auto elseStatement = std::make_shared<ElseStatement>(ln);
            elseStatement->withToken(m_tm.currentToken());
            m_tm.advanceTokenIterator();
            if(m_tm.currentToken().lexeme != Lexeme::OPEN_CURLY) { return nullptr; }
            m_tm.advanceTokenIterator();
            while(m_tm.currentToken().lexeme != Lexeme::CLOSE_CURLY) {
                auto statement = buildStatement();
                if(statement) {
                    elseStatement->addBodyStatement(std::move(statement));
                }
                m_tm.advanceTokenIterator();
            }
            ifStatement->withElsePart(std::move(elseStatement));
        }
        return ifStatement;
    }

    std::shared_ptr<Statement> Parser::parseStartStatement()
    {
        if(m_tm.currentToken().raw != "start") { return nullptr; }
        auto const ln = m_tm.currentToken().lineNumber;
        auto startStatement = std::make_shared<StartStatement>(ln);
        startStatement->withToken(m_tm.currentToken());
        m_tm.advanceTokenIterator();
        if(m_tm.currentToken().lexeme != Lexeme::OPEN_CURLY) { return nullptr; }
        m_tm.advanceTokenIterator();
        while(m_tm.currentToken().lexeme != Lexeme::CLOSE_CURLY) {
            auto statement = buildStatement();
            if(statement) {
                startStatement->addBodyStatement(std::move(statement));
            }
            m_tm.advanceTokenIterator();
        }
        m_startStatement = startStatement;
        return startStatement;
    }

    std::shared_ptr<Statement> Parser::parseAsyncStatement()
    {
        if(m_tm.currentToken().raw != "async") { return nullptr; }
        auto const ln = m_tm.currentToken().lineNumber;
        auto asyncStatement = std::make_shared<AsyncStatement>(ln);
        asyncStatement->withToken(m_tm.currentToken());
        m_tm.advanceTokenIterator();
        if(m_tm.currentToken().lexeme != Lexeme::OPEN_CURLY) { return nullptr; }
        m_tm.advanceTokenIterator();
        while(m_tm.currentToken().lexeme != Lexeme::CLOSE_CURLY) {
            auto statement = buildStatement();
            if(statement) {
                asyncStatement->addBodyStatement(std::move(statement));
            }
            m_tm.advanceTokenIterator();
        }
        return asyncStatement;
    }

    std::shared_ptr<Statement> Parser::parseFunctionStatement()
    {
        if(m_tm.currentToken().raw != "fn") { return nullptr; }
        auto const ln = m_tm.currentToken().lineNumber;
        auto functionStatement = std::make_shared<FunctionStatement>(ln);
        functionStatement->withToken(m_tm.currentToken());
        m_tm.advanceTokenIterator();
        if(m_tm.currentToken().lexeme != Lexeme::GENERIC_STRING) { return nullptr; }
        auto const theName = m_tm.currentToken();
        functionStatement->withNameToken(theName);
        m_tm.advanceTokenIterator();
        auto collection = parseExpressionCollectionExpression(true /* ident only */);
        if(!collection) { return nullptr; }
        functionStatement->withExpressionCollection(std::move(collection));
        m_tm.advanceTokenIterator();
        // Handle 'arrow-less' condition (not returning call).
        if(m_tm.currentToken().lexeme != Lexeme::ARROW) {
            functionStatement->withReturnIdentifierToken(Token{Lexeme::NIL, "nil", m_tm.currentToken().lineNumber});
        } else {
            m_tm.advanceTokenIterator();
            if(m_tm.currentToken().lexeme != Lexeme::GENERIC_STRING) { return nullptr; }
            functionStatement->withReturnIdentifierToken(m_tm.currentToken());
            m_tm.advanceTokenIterator();
        }
        if(m_tm.currentToken().lexeme != Lexeme::OPEN_CURLY) { return nullptr; }
        m_tm.advanceTokenIterator();
        while(m_tm.currentToken().lexeme != Lexeme::CLOSE_CURLY) {
            auto statement = buildStatement();
            if(statement) {
                functionStatement->addBodyStatement(std::move(statement));
            }
            m_tm.advanceTokenIterator();
        }

        m_functions.emplace(theName.raw, functionStatement);
        return functionStatement;
    }

    std::shared_ptr<Statement> Parser::parseSingleExpressionStatement()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        auto singleExpressionStatement = std::make_shared<SingleExpressionStatement>(ln);
        auto const expression = parseExpression();
        if(!expression) {
            return nullptr;
        }
        singleExpressionStatement->withExpression(expression);
        m_tm.advanceTokenIterator();
        if(m_tm.currentToken().lexeme != Lexeme::SEMICOLON) {
            return nullptr;
        }
        return singleExpressionStatement;
    }
}
