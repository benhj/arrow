/// (c) Ben Jones 2019 - present

/// This
#include "Parser.hpp"
#include "ExpressionParser.hpp"

/// Receivers
#include "receivers/DollarIdentifierReceiver.hpp"
#include "receivers/IdentifierReceiver.hpp"
#include "receivers/ArrayAccessorReceiver.hpp"
#include "builtin/filesystem/FileReceiver.hpp"

/// Statements
#include "statements/AnsiStatement.hpp"
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
#include "statements/LoopBreakStatement.hpp"
#include "statements/ReleaseStatement.hpp"
#include "statements/RepeatStatement.hpp"
#include "statements/ReturnStatement.hpp"
#include "statements/ScopedBlockStatement.hpp"
#include "statements/SimpleArrowStatement.hpp"
#include "statements/SingleExpressionStatement.hpp"
#include "statements/StartStatement.hpp"
#include "statements/StringToIntStatement.hpp"
#include "statements/WhileStatement.hpp"

/// Other
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "statements/evaluator/StatementEvaluator.hpp"
#include "LanguageException.hpp"
#include "lexer/Lexeme.hpp"
#include <algorithm>
#include <functional>
#include <utility>

namespace arrow {

    std::map<std::string, std::shared_ptr<FunctionStatement>> Parser::m_functions{};

    Parser::Parser(std::vector<Token> tokens, std::ostream & os)
      : m_tm(std::move(tokens))
      , m_os(os)
      , m_ep(m_tm)
      , m_rp(m_tm)
      , m_startStatement(nullptr)
      , m_statements()
    {
        m_functions.clear();
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
            auto expression = m_ep.parseExpression();
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
            static std::vector<std::function<std::shared_ptr<Statement>(Parser*)>> pvec{
                [](Parser* t){return t->parseBreakStatement();},
                [](Parser* t){return t->parseReturnStatement();},
                [](Parser* t){return t->parseFunctionStatement();},
                [](Parser* t){return t->parseSimpleArrowStatement();},
                [](Parser* t){return t->parseArrowStatement();},
                [](Parser* t){return t->parseReleaseStatement();},
                [](Parser* t){return t->parseSingleExpressionStatement();},
                [](Parser* t){return t->parseArrowlessStatement();},
                [](Parser* t){return t->parseRepeatStatement();},
                [](Parser* t){return t->parseWhileStatement();},
                [](Parser* t){return t->parseForStatement();},
                [](Parser* t){return t->parseIfStatement();},
                [](Parser* t){return t->parseStartStatement();},
                [](Parser* t){return t->parseAsyncStatement();},
            };
            for(auto const & p : pvec) {
                auto store = m_tm.retrieveIt();
                auto statement = p(this);
                if(statement) {
                    return statement;
                }
                m_tm.revert(store);
            }
        }
        throw LanguageException("Unable to parse statement", m_tm.currentToken().lineNumber);
    }

    std::shared_ptr<Statement> Parser::parseScopedBlockStatement()
    {
        if(m_tm.currentToken().lexeme != Lexeme::OPEN_CURLY) {
            return nullptr;
        }
        m_tm.advanceTokenIterator();
        auto const ln = m_tm.currentToken().lineNumber;
        auto sbs = std::make_shared<ScopedBlockStatement>(ln, m_os);
        while(m_tm.currentToken().lexeme != Lexeme::CLOSE_CURLY) {
            auto statement = buildStatement();
            if(statement) {
                sbs->addBodyStatement(std::move(statement));
            }
            m_tm.advanceTokenIterator();
        }
        return sbs;
    }

    std::shared_ptr<Statement> Parser::parseArrowStatement()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        // Don't attempt to parse function statements
        if(m_tm.currentToken().raw == "fn") {
            return nullptr;
        }
        auto arrowStatement = std::make_shared<ArrowStatement>(ln, m_os);
        arrowStatement->withToken(m_tm.currentToken());
        auto const keyword = m_tm.currentToken().raw;
        m_tm.advanceTokenIterator();
        
        auto expression = m_ep.parseExpression();
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
                                    return std::make_shared<StringToIntStatement>(ln, *arrowStatement, m_os);
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
        auto arrowStatement = std::make_shared<SimpleArrowStatement>(ln, m_os);
        auto expression = m_ep.parseExpression();
        if(expression) {
            arrowStatement->withExpression(std::move(expression));
            m_tm.advanceTokenIterator();
            if(m_tm.notAtEnd()) {
                if(m_tm.currentToken().lexeme != Lexeme::ARROW) {
                    return nullptr;
                }
                m_tm.advanceTokenIterator();
                if(m_tm.notAtEnd()) {
                    auto receiver = m_rp.parseReceiver();
                    if(!receiver) {
                        return nullptr;
                    }
                    arrowStatement->withIdentifier(std::move(receiver));
                    m_tm.advanceTokenIterator();
                    if(m_tm.notAtEnd()) {
                        if(m_tm.currentToken().lexeme == Lexeme::SEMICOLON) {
                            return arrowStatement;
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
        auto arrowlessStatement = std::make_shared<ArrowlessStatement>(ln, m_os);
        arrowlessStatement->withToken(m_tm.currentToken());
        m_tm.advanceTokenIterator();
        auto const keyword = arrowlessStatement->getToken().raw;
        if(keyword == "erase") {
            auto expression = m_ep.parseIndexExpression();
            if(!expression) {
                throw LanguageException("Not an index expression", ln);
            }
            arrowlessStatement->withExpression(std::move(expression));
            m_tm.advanceTokenIterator();
            if(m_tm.currentToken().lexeme == Lexeme::SEMICOLON) {
                return std::make_shared<EraseStatement>(ln, std::move(arrowlessStatement), m_os);
            }
        } else {
            auto expression = m_ep.parseExpression();
            if(expression) {
                arrowlessStatement->withExpression(std::move(expression));
                m_tm.advanceTokenIterator();
                if(m_tm.notAtEnd()) {
                    if(m_tm.currentToken().lexeme == Lexeme::SEMICOLON) {
                        
                        if(keyword == "prn" || keyword == "pr" || keyword == "say") {
                            return std::make_shared<EchoStatement>(ln, std::move(arrowlessStatement), m_os);
                        } else if(keyword == "exit") {
                            return std::make_shared<ExitStatement>(ln, std::move(arrowlessStatement), m_os);
                        } else if(keyword == "ansi_up") {
                            return std::make_shared<AnsiStatement>(ln, std::move(arrowlessStatement), m_os);
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
        auto releaseStatement = std::make_shared<ReleaseStatement>(ln, m_os);
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
        if(m_tm.currentToken().raw != "break") {
            return nullptr;
        }
        auto const ln = m_tm.currentToken().lineNumber;
        auto breakStatement = std::make_shared<LoopBreakStatement>(ln, m_os);
        m_tm.advanceTokenIterator();
        if(m_tm.currentToken().lexeme == Lexeme::SEMICOLON) {
            return breakStatement;
        }
        return nullptr;
    }

    std::shared_ptr<Statement> Parser::parseReturnStatement()
    {
        if(m_tm.currentToken().raw != "return") {
            return nullptr;
        }
        auto const ln = m_tm.currentToken().lineNumber;
        auto returnStatement = std::make_shared<ReturnStatement>(ln, m_os);
        m_tm.advanceTokenIterator();
        if(m_tm.currentToken().lexeme == Lexeme::SEMICOLON) {
            return returnStatement;
        }
        return nullptr;
    }

    std::shared_ptr<Statement> Parser::parseRepeatStatement()
    {
        if(m_tm.currentToken().raw != "repeat") {
            return nullptr;
        }
        auto const ln = m_tm.currentToken().lineNumber;
        auto repeatStatement = std::make_shared<RepeatStatement>(ln, m_os);
        repeatStatement->withToken(m_tm.currentToken());
        m_tm.advanceTokenIterator();
        auto expression = m_ep.parseExpression();
        if(expression) {
            repeatStatement->withExpression(std::move(expression));
            m_tm.advanceTokenIterator();
            if(m_tm.currentToken().raw != "times") {
                return nullptr;
            }
            m_tm.advanceTokenIterator();
            auto innerStatement = parseScopedBlockStatement();
            if(!innerStatement) {
                return nullptr;
            }
            repeatStatement->withInnerStatement(std::move(innerStatement));
        }
        return repeatStatement;
    }

    std::shared_ptr<Statement> Parser::parseWhileStatement()
    {
        if(m_tm.currentToken().raw != "while") {
            return nullptr;
        }
        auto const ln = m_tm.currentToken().lineNumber;
        auto whileStatement = std::make_shared<WhileStatement>(ln, m_os);
        whileStatement->withToken(m_tm.currentToken());
        m_tm.advanceTokenIterator();
        if(m_tm.currentToken().lexeme != Lexeme::OPEN_PAREN) {
            return nullptr;
        }
        auto expression = m_ep.parseGroupedExpression();
        if(expression) {
            whileStatement->withExpression(std::move(expression));
            m_tm.advanceTokenIterator();
            auto innerStatement = parseScopedBlockStatement();
            if(!innerStatement) {
                return nullptr;
            }
            whileStatement->withInnerStatement(std::move(innerStatement));
        }
        return whileStatement;
    }

    std::shared_ptr<Statement> Parser::parseForStatement()
    {
        if(m_tm.currentToken().raw != "for") {
            return nullptr;
        }
        auto const ln = m_tm.currentToken().lineNumber;
        auto forStatement = std::make_shared<ForStatement>(ln, m_os);
        forStatement->withToken(m_tm.currentToken());
        m_tm.advanceTokenIterator();
        if(m_tm.currentToken().lexeme != Lexeme::GENERIC_STRING) {
            return nullptr;
        }
        forStatement->addIndexer(m_tm.currentToken());
        m_tm.advanceTokenIterator();
        while(m_tm.currentToken().lexeme == Lexeme::COMMA) {
            if(m_tm.nextToken().lexeme != Lexeme::GENERIC_STRING) {
                return nullptr;
            }
            forStatement->addIndexer(m_tm.nextToken());
            m_tm.advanceTokenIterator();
            m_tm.advanceTokenIterator();
        }
        if(m_tm.currentToken().raw != "in") {
            return nullptr;
        }
        m_tm.advanceTokenIterator();
        if(m_tm.currentToken().lexeme != Lexeme::GENERIC_STRING) {
            return nullptr;
        }
        forStatement->withIdentifier(m_tm.currentToken());
        m_tm.advanceTokenIterator();
        auto innerStatement = parseScopedBlockStatement();
        if(!innerStatement) {
            return nullptr;
        }
        forStatement->withInnerStatement(std::move(innerStatement));
        return forStatement;
     }

    std::shared_ptr<Statement> Parser::parseIfStatement()
    {
        if(m_tm.currentToken().raw != "if") {
            return nullptr;
        }
        auto ln = m_tm.currentToken().lineNumber;
        auto ifStatement = std::make_shared<IfStatement>(ln, m_os);
        ifStatement->withToken(m_tm.currentToken());
        if(m_tm.nextToken().lexeme != Lexeme::OPEN_PAREN) {
            return nullptr;
        }
        m_tm.advanceTokenIterator();
        {
            auto expression = m_ep.parseGroupedExpression();
            if(!expression) {
                return nullptr;
            }
            ifStatement->withExpression(expression);
        }
        m_tm.advanceTokenIterator();
        auto innerStatement = parseScopedBlockStatement();
        if(!innerStatement) {
            return nullptr;
        }
        ifStatement->withInnerStatement(std::move(innerStatement));

        while(m_tm.nextToken().raw == "elseif") {
            m_tm.advanceTokenIterator();
            ln = m_tm.currentToken().lineNumber;
            auto elseIfStatement = std::make_shared<ElseIfStatement>(ln, m_os);
            elseIfStatement->withToken(m_tm.currentToken());
            if(m_tm.nextToken().lexeme != Lexeme::OPEN_PAREN) {
                return nullptr;
            }
            m_tm.advanceTokenIterator();
            {
                auto expression = m_ep.parseGroupedExpression();
                if(!expression) {
                    return nullptr;
                }
                elseIfStatement->withExpression(expression);
            }
            m_tm.advanceTokenIterator();
            auto innerStatement = parseScopedBlockStatement();
            if(!innerStatement) {
                return nullptr;
            }
            elseIfStatement->withInnerStatement(std::move(innerStatement));
            ifStatement->addElseIfPart(std::move(elseIfStatement));
        }
        if(m_tm.nextToken().raw == "else") {
            ln = m_tm.currentToken().lineNumber;
            m_tm.advanceTokenIterator();
            auto elseStatement = std::make_shared<ElseStatement>(ln, m_os);
            elseStatement->withToken(m_tm.currentToken());
            m_tm.advanceTokenIterator();
            auto innerStatement = parseScopedBlockStatement();
            if(!innerStatement) {
                return nullptr;
            }
            elseStatement->withInnerStatement(std::move(innerStatement));
            ifStatement->withElsePart(std::move(elseStatement));
        }
        return ifStatement;
    }

    std::shared_ptr<Statement> Parser::parseStartStatement()
    {
        if(m_tm.currentToken().raw != "start") {
            return nullptr;
        }
        auto const ln = m_tm.currentToken().lineNumber;
        auto startStatement = std::make_shared<StartStatement>(ln, m_os);
        startStatement->withToken(m_tm.currentToken());
        m_tm.advanceTokenIterator();
        auto innerStatement = parseScopedBlockStatement();
        if(!innerStatement) {
            return nullptr;
        }
        startStatement->withInnerStatement(std::move(innerStatement));
        m_startStatement = startStatement;
        return startStatement;
    }

    std::shared_ptr<Statement> Parser::parseAsyncStatement()
    {
        if(m_tm.currentToken().raw != "async") {
            return nullptr;
        }
        auto const ln = m_tm.currentToken().lineNumber;
        auto asyncStatement = std::make_shared<AsyncStatement>(ln, m_os);
        asyncStatement->withToken(m_tm.currentToken());
        m_tm.advanceTokenIterator();
        if(m_tm.currentToken().lexeme != Lexeme::OPEN_CURLY) {
            return nullptr;
        }
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
        if(m_tm.currentToken().raw != "fn") {return nullptr;
        }
        auto const ln = m_tm.currentToken().lineNumber;
        auto functionStatement = std::make_shared<FunctionStatement>(ln, m_os);
        functionStatement->withToken(m_tm.currentToken());
        m_tm.advanceTokenIterator();
        if(m_tm.currentToken().lexeme != Lexeme::GENERIC_STRING) {
            return nullptr;
        }
        auto const theName = m_tm.currentToken();
        functionStatement->withNameToken(theName);
        m_tm.advanceTokenIterator();
        auto collection = m_ep.parseExpressionCollectionExpression(true /* ident only */);
        if(!collection) {
            return nullptr;
        }
        functionStatement->withExpressionCollection(std::move(collection));
        m_tm.advanceTokenIterator();
        // Handle 'arrow-less' condition (not returning call).
        if(m_tm.currentToken().lexeme != Lexeme::ARROW) {
            functionStatement->withReturnIdentifierToken(Token{Lexeme::NIL, "nil", m_tm.currentToken().lineNumber});
        } else {
            m_tm.advanceTokenIterator();
            if(m_tm.currentToken().lexeme != Lexeme::GENERIC_STRING) {
                return nullptr;
            }
            functionStatement->withReturnIdentifierToken(m_tm.currentToken());
            m_tm.advanceTokenIterator();
        }
        auto innerStatement = parseScopedBlockStatement();
        if(!innerStatement) {
            return nullptr;
        }
        functionStatement->withInnerStatement(std::move(innerStatement));
        m_functions.emplace(theName.raw, functionStatement);
        return functionStatement;
    }

    std::shared_ptr<Statement> Parser::parseSingleExpressionStatement()
    {
        auto const ln = m_tm.currentToken().lineNumber;
        auto singleExpressionStatement = std::make_shared<SingleExpressionStatement>(ln, m_os);
        auto const expression = m_ep.parseExpression();
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
