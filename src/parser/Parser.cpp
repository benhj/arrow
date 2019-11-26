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

/// Expressions
#include "expressions/BooleanExpression.hpp"
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

/// Other
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
                std::cout<<currentToken().lexeme<<std::endl;
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

        // First try and parse a statement of the form
        // 1 -> a;
        // a + 1 -> b;
        // etc.
        // In contrast to most statement, this doesn't
        // begin with an identifier so we need to try
        // and parse these first.
        auto store = m_current;
        auto statement = parseSimpleArrowStatement();
        if(statement) { 
            return statement;
        }
        // Revert token iterator state in case of failure
        m_current = store;

        // If that fails, try parsing a statement beginning
        // with a keyword
        if(!statement && currentToken().lexeme == Lexeme::GENERIC_STRING) {

            static std::vector<std::function<std::shared_ptr<Statement>(void)>> pvec;
            if(pvec.empty()) {
                pvec.emplace_back([this]{return parseArrowStatement();});
                pvec.emplace_back([this]{return parseArrowlessStatement();});
                pvec.emplace_back([this]{return parseRepeatStatement();});
                pvec.emplace_back([this]{return parseWhileStatement();});
                pvec.emplace_back([this]{return parseForStatement();});
                pvec.emplace_back([this]{return parseIfStatement();});
                pvec.emplace_back([this]{return parseCallStatement();});
                pvec.emplace_back([this]{return parseStartStatement();});
                pvec.emplace_back([this]{return parseFunctionStatement();});
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
        std::string error("Unable to parse statement on line ");
        error.append(std::to_string(currentToken().lineNumber));
        throw std::runtime_error(error);
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
        exp->withIdentifierToken(currentToken());
        return exp;
    }

    std::shared_ptr<Expression> Parser::parseIndexExpression()
    {
        if(nextToken().lexeme != Lexeme::OPEN_SQUARE) {
            return nullptr;
        }
        auto exp = std::make_shared<IndexExpression>();

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
        auto exp = std::make_shared<ListWordExpression>();
        exp->withWordToken(currentToken());
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

    std::shared_ptr<Expression> Parser::parseBooleanExpression()
    {
        auto exp = std::make_shared<BooleanExpression>();
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
        auto exp = std::make_shared<MathExpression>();
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
            if(isMathOperator(nextToken().lexeme)) {
                auto extra = std::make_shared<MathExpression>();
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
                auto extra = std::make_shared<BooleanExpression>();
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

    std::shared_ptr<Expression> 
    Parser::parseExpressionCollectionExpression(bool const identifierOnly)
    {
        if(currentToken().lexeme != Lexeme::OPEN_PAREN) { return nullptr; }
        auto expression = std::make_shared<ExpressionCollectionExpression>();
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
        } 
        return nullptr;
    }

    std::shared_ptr<Expression> Parser::parseExpression(bool checkOperator)
    {
        if(m_current + 1 != std::end(m_tokens)) {
            if (checkOperator && isBooleanOperator(nextToken().lexeme)) {
                return parseBooleanExpression();
            } else if(checkOperator && isMathOperator(nextToken().lexeme))  {
                return parseMathExpression();
            }  else if(currentToken().lexeme == Lexeme::OPEN_PAREN) {

                auto store = m_current;
                auto exp = parseGroupedExpression();
                if(!exp) {
                    m_current = store;
                    // try (a, b, c) collection
                    return parseExpressionCollectionExpression();
                }
                return exp;
            } else if(currentToken().lexeme == Lexeme::OPEN_SQUARE) {
                return parseListExpression();
            } else if(currentToken().lexeme == Lexeme::INTEGER_NUM) {
                return parseLiteralIntExpression();
            } else if(currentToken().lexeme == Lexeme::REAL_NUM) {
                return parseLiteralRealExpression();
            } else if(currentToken().lexeme == Lexeme::GENERIC_STRING) {
                auto exp = parseIndexExpression();
                auto store = m_current;
                if(!exp) {
                    return parseIdentifierExpression();
                }
                m_current = store;
                return exp;
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
        }
        return nullptr;
    }

    std::shared_ptr<Statement> Parser::parseArrowStatement()
    {
        auto arrowStatement = std::make_shared<ArrowStatement>();
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
                                    return std::make_shared<StringToIntStatement>(*arrowStatement);
                                } else if(keyword == "arg") {
                                    return std::make_shared<ArgStatement>(*arrowStatement);
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
        auto arrowStatement = std::make_shared<SimpleArrowStatement>();
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
        auto arrowlessStatement = std::make_shared<ArrowlessStatement>();
        arrowlessStatement->withToken(currentToken());
        advanceTokenIterator();
        auto expression = parseExpression();
        if(expression) {
            arrowlessStatement->withExpression(std::move(expression));
            advanceTokenIterator();
            if(notAtEnd()) {
                if(currentToken().lexeme == Lexeme::SEMICOLON) {
                    auto const keyword = arrowlessStatement->getToken().raw;
                    if(keyword == "prn" || keyword == "pr" || keyword == "say") {
                        return std::make_shared<EchoStatement>(std::move(arrowlessStatement));
                    } else if(keyword == "exit") {
                        return std::make_shared<ExitStatement>(std::move(arrowlessStatement));
                    } else if(keyword == "ansi_up") {
                        return std::make_shared<AnsiStatement>(std::move(arrowlessStatement));
                    }
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
        m_startStatement = startStatement;
        return startStatement;
    }

    std::shared_ptr<Statement> Parser::parseFunctionStatement()
    {
        if(currentToken().raw != "fn") { return nullptr; }
        auto functionStatement = std::make_shared<FunctionStatement>();
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
}
