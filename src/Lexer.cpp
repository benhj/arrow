#include "Lexer.hpp"
#include <iterator>
#include <cctype>
#include <cstdio>
#include <string>

namespace jasl {

    bool checkIfDoubleCharToken(char const c, std::istream & stream, std::vector<Token> & tokens) {
        switch(c) {
            case '-':
            {
                auto next = stream.peek();
                if(next != EOF && char(next) == '>') {
                    stream.get(); // iterate
                    tokens.emplace_back(Lexeme::ARROW, "->");
                    return true;
                }
                tokens.emplace_back(Lexeme::MINUS, "-");
                return false;
            }
            case '/':
            {
                auto next = stream.peek();
                if(next != EOF && char(next) == '=') {
                    stream.get(); // iterate
                    tokens.emplace_back(Lexeme::NOT_EQUAL, "/=");
                    return true;
                }
                tokens.emplace_back(Lexeme::DIVIDE, "/");
                return false;
            }
            case '>':
            {
                auto next = stream.peek();
                if(next != EOF && char(next) == '=') {
                    stream.get(); // iterate
                    tokens.emplace_back(Lexeme::GREATER_EQUAL, ">=");
                    return true;
                }
                tokens.emplace_back(Lexeme::GREATER, ">");
                return false;
            }
            case '<':
            {
                auto next = stream.peek();
                if(next != EOF && char(next) == '=') {
                    stream.get(); // iterate
                    tokens.emplace_back(Lexeme::LESS_EQUAL, "<=");
                    return true;
                }
                tokens.emplace_back(Lexeme::LESS, "<");
                return false;
            }
            default: return false;
        }
        return false;
    }

    bool checkIfSingleCharToken(char const c, std::vector<Token> & tokens) {
        switch (c) {
            case '+': tokens.emplace_back(Lexeme::PLUS, "+"); break;
            case '*': tokens.emplace_back(Lexeme::STAR, "*"); break;
            case '=': tokens.emplace_back(Lexeme::EQUAL, "="); break;
            case '{': tokens.emplace_back(Lexeme::OPEN_CURLY, "{"); break;
            case '}': tokens.emplace_back(Lexeme::CLOSE_CURLY, "}"); break;
            case '[': tokens.emplace_back(Lexeme::OPEN_SQUARE, "["); break;
            case ']': tokens.emplace_back(Lexeme::CLOSE_SQUARE, "]"); break;
            case '(': tokens.emplace_back(Lexeme::OPEN_PAREN, "("); break;
            case ')': tokens.emplace_back(Lexeme::CLOSE_PAREN, ")"); break;
            case ':': tokens.emplace_back(Lexeme::COLON, ":"); break;
            default : return false;
        };
        return true;
    }

    std::string getAlphaNumsUntilSpace(std::istream & stream) {
        std::string dat;
        auto next = stream.peek();
        while (next != EOF && (std::isalnum(next) || next == '_')) {
            stream.get(); // iterate
            dat.push_back(next);
            next = stream.peek();
        }
        return dat;
    }

    bool checkIfKeyword(std::string const & word, std::vector<Token> & tokens) {

        static std::vector<Token> poss {
            {Lexeme::IF, "if"}, {Lexeme::FOR ,"for"}, 
            {Lexeme::EACH, "each"}, {Lexeme::REPEAT, "repeat"}, 
            {Lexeme::WHILE, "while"}, {Lexeme::TIMES, "times"},
            {Lexeme::START, "start"}, {Lexeme::FN, "fn"}, 
            {Lexeme::BOOL, "bool"}, {Lexeme::INTEGER, "int"},
            {Lexeme::REAL, "real"}, {Lexeme::STRING, "string"}, 
            {Lexeme::IN, "in"}, {Lexeme::LIST, "list"}, 
            {Lexeme::SAY, "say"}, {Lexeme::PR, "pr"},
            {Lexeme::PRN, "prn"}, {Lexeme::TRUE, "true"}, 
            {Lexeme::FALSE, "false"}, {Lexeme::INPUT, "input"}
        };
        auto found = std::find_if(std::begin(poss), std::end(poss),
            [&word](Token const & token){ return token.raw == word; });
        if(found != std::end(tokens)) {
            tokens.push_back(*found);
            return true;
        }
        return false;
    }

    bool checkIfMultiAlphaToken(char const c, std::istream & stream, std::vector<Token> & tokens) {
        // check if beginning of 'generic string'
        if(std::isalpha(c) || c == '_' /* also permit */) {
            std::string dat;
            dat.push_back(c);
            auto rest = getAlphaNumsUntilSpace(stream);
            dat.append(rest);
            if(!checkIfKeyword(dat, tokens)) {
                tokens.emplace_back(Lexeme::GENERIC_STRING, std::move(dat));
            }
            return true;
        } else if(c == '^') {
            auto next = stream.peek();
            if(next != EOF) {
                // see if ^str
                if(std::isalnum(next)) {
                    auto dat = getAlphaNumsUntilSpace(stream);
                    tokens.emplace_back(Lexeme::HAT, std::move(dat));
                    return true;
                } 
                // see if ^^str
                else if(next == '^') {
                    if(std::isalnum(next)) {
                        auto dat = getAlphaNumsUntilSpace(stream);
                        tokens.emplace_back(Lexeme::HAT_HAT, std::move(dat));
                        return true;
                    }
                }
            }
        } else if(c == '?') {
            auto next = stream.peek();
            if(next != EOF) {
                // see if ?str
                if(std::isalnum(next)) {
                    stream.get();
                    auto dat = getAlphaNumsUntilSpace(stream);
                    tokens.emplace_back(Lexeme::QUESTION, std::move(dat));
                    return true;
                } 
                // see if ??str
                else if(next == '?') {
                    if(std::isalnum(next)) {
                        auto dat = getAlphaNumsUntilSpace(stream);
                        tokens.emplace_back(Lexeme::QUESTION_QUESTION, std::move(dat));
                        return true;
                    }
                }
            }
        } else if(c == ';') {
            auto next = stream.peek();
            if(next != EOF && next == ';') {
                stream.get();
                auto next = stream.peek();
                if(next != EOF && next == ';') {
                    stream.get();
                    char com;
                    std::string comment;
                    while(stream.get(com) && com != '\n') {
                        comment.push_back(com);
                    }
                    tokens.emplace_back(Lexeme::COMMENT, std::move(comment));
                    return true;
                }
            }
            tokens.emplace_back(Lexeme::SEMICOLON, ";");
            return true;
        }
        return false;
    }

    std::vector<Token> Lexer::tokenize(std::istream & stream) const {
        std::vector<Token> tokens;
        char c;
        while(stream.get(c)) {
            if(c == ' ') { continue; }
            if(c == '\n') { continue; }
            if(checkIfDoubleCharToken(c, stream, tokens)) { continue; }
            if(checkIfSingleCharToken(c, tokens)) { continue; }
            if(checkIfMultiAlphaToken(c, stream, tokens)) { continue; }
        }
        return tokens;
    }

}