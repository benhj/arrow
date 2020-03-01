/// (c) Ben Jones 2019 - present

#include "Lexer.hpp"
#include <algorithm>
#include <iterator>
#include <cctype>
#include <cstdio>
#include <string>

namespace arrow {

    bool checkIfDoubleCharToken(char const c, 
                                std::istream & stream, 
                                std::vector<Token> & tokens,
                                long const lineNumber) {
        switch(c) {
            case '-':
            {
                auto next = stream.peek();
                if(next != EOF && char(next) == '>') {
                    stream.get(); // iterate
                    tokens.emplace_back(Lexeme::ARROW, "->", lineNumber);
                    return true;
                }
                tokens.emplace_back(Lexeme::MINUS, "-", lineNumber);
                return true;
            }
            case '/':
            {
                auto next = stream.peek();
                if(next != EOF && char(next) == '=') {
                    stream.get(); // iterate
                    tokens.emplace_back(Lexeme::NOT_EQUAL, "/=", lineNumber);
                    return true;
                }
                tokens.emplace_back(Lexeme::DIVIDE, "/", lineNumber);
                return true;
            }
            case '>':
            {
                auto next = stream.peek();
                if(next != EOF && char(next) == '=') {
                    stream.get(); // iterate
                    tokens.emplace_back(Lexeme::GREATER_EQUAL, ">=", lineNumber);
                    return true;
                }
                tokens.emplace_back(Lexeme::GREATER, ">", lineNumber);
                return true;
            }
            case '<':
            {
                auto next = stream.peek();
                if(next != EOF && char(next) == '=') {
                    stream.get(); // iterate
                    tokens.emplace_back(Lexeme::LESS_EQUAL, "<=", lineNumber);
                    return true;
                }
                tokens.emplace_back(Lexeme::LESS, "<", lineNumber);
                return true;
            }
            case '|':
            {
                auto next = stream.peek();
                if(next != EOF && char(next) == '|') {
                    stream.get(); // iterate
                    tokens.emplace_back(Lexeme::OR, "||", lineNumber);
                    return true;
                }
                return false;
            }
            case '&':
            {
                auto next = stream.peek();
                if(next != EOF && char(next) == '&') {
                    stream.get(); // iterate
                    tokens.emplace_back(Lexeme::AND, "&&", lineNumber);
                    return true;
                }
                return false;
            }
            case '=':
            {
                auto next = stream.peek();
                if(next != EOF && char(next) == '=') {
                    stream.get(); // iterate
                    tokens.emplace_back(Lexeme::EQUAL_EQUAL, "==", lineNumber);
                    return true;
                }
                tokens.emplace_back(Lexeme::EQUAL, "=", lineNumber);
                return true;
            }
            default: return false;
        }
        return false;
    }

    bool checkIfSingleCharToken(char const c, 
                                std::vector<Token> & tokens,
                                long const lineNumber) {
        switch (c) {
            case '+': tokens.emplace_back(Lexeme::PLUS, "+", lineNumber); break;
            case '*': tokens.emplace_back(Lexeme::STAR, "*", lineNumber); break;
            case '%': tokens.emplace_back(Lexeme::PERCENT, "%", lineNumber); break;
            case '{': tokens.emplace_back(Lexeme::OPEN_CURLY, "{", lineNumber); break;
            case '}': tokens.emplace_back(Lexeme::CLOSE_CURLY, "}", lineNumber); break;
            case '[': tokens.emplace_back(Lexeme::OPEN_SQUARE, "[", lineNumber); break;
            case ']': tokens.emplace_back(Lexeme::CLOSE_SQUARE, "]", lineNumber); break;
            case '(': tokens.emplace_back(Lexeme::OPEN_PAREN, "(", lineNumber); break;
            case ')': tokens.emplace_back(Lexeme::CLOSE_PAREN, ")", lineNumber); break;
            case ':': tokens.emplace_back(Lexeme::COLON, ":", lineNumber); break;
            case ',': tokens.emplace_back(Lexeme::COMMA, ",", lineNumber); break;
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

    bool checkIfKeyword(std::string const & word, 
                        std::vector<Token> & tokens,
                        long const lineNumber) {

        static std::vector<Token> poss {
            {Lexeme::IF, "if", lineNumber}, {Lexeme::FOR ,"for", lineNumber}, 
            {Lexeme::EACH, "each", lineNumber}, {Lexeme::REPEAT, "repeat", lineNumber}, 
            {Lexeme::WHILE, "while", lineNumber}, {Lexeme::TIMES, "times", lineNumber},
            {Lexeme::START, "start", lineNumber}, {Lexeme::FN, "fn", lineNumber}, 
            {Lexeme::BOOL, "bool", lineNumber}, {Lexeme::INTEGER, "int", lineNumber},
            {Lexeme::REAL, "real", lineNumber}, {Lexeme::STRING, "string", lineNumber},
            {Lexeme::PUT, "put", lineNumber}, {Lexeme::ELSE, "else", lineNumber}, 
            {Lexeme::IN, "in", lineNumber}, {Lexeme::LIST, "list", lineNumber}, 
            {Lexeme::SAY, "say", lineNumber}, {Lexeme::PR, "pr", lineNumber},
            {Lexeme::PRN, "prn", lineNumber}, {Lexeme::TRUE, "true", lineNumber}, 
            {Lexeme::FALSE, "false", lineNumber}, {Lexeme::INPUT, "input", lineNumber},
            {Lexeme::CALL, "call", lineNumber}
        };
        auto found = std::find_if(std::begin(poss), std::end(poss),
            [&word](Token const & token){ return token.raw == word; });
        if(found != std::end(poss)) {
            tokens.push_back(*found);
            return true;
        }
        return false;
    }

    bool checkIfMultiAlphaToken(char const c, 
                                std::istream & stream, 
                                std::vector<Token> & tokens,
                                long & lineNumber) {
        // check if beginning of 'generic string'
        if(std::isalpha(c) || c == '_' /* also permit */) {
            std::string dat;
            dat.push_back(c);
            auto rest = getAlphaNumsUntilSpace(stream);
            dat.append(rest);
            //if(!checkIfKeyword(dat, tokens)) {
            tokens.emplace_back(Lexeme::GENERIC_STRING, std::move(dat), lineNumber);
            //}
            return true;
        } else if(c == '^') {
            auto next = stream.peek();
            if(next != EOF) {
                // see if ^str
                if(std::isalnum(next)) {
                    auto dat = getAlphaNumsUntilSpace(stream);
                    tokens.emplace_back(Lexeme::HAT_STRING, std::move(dat), lineNumber);
                    return true;
                } 
                // see if ^^str
                else if(next == '^') {
                    stream.get();
                    next = stream.peek();
                    if(std::isalnum(next)) {
                        auto dat = getAlphaNumsUntilSpace(stream);
                        tokens.emplace_back(Lexeme::HAT_HAT_STRING, std::move(dat), lineNumber);
                        return true;
                    }
                } else {
                    std::string dat("^");
                    tokens.emplace_back(Lexeme::XOR, std::move(dat), lineNumber);
                }
            }
        } else if(c == '?') {
            auto next = stream.peek();
            if(next != EOF) {
                // see if ?str
                if(std::isalnum(next)) {
                    auto dat = getAlphaNumsUntilSpace(stream);
                    tokens.emplace_back(Lexeme::Q_STRING, std::move(dat), lineNumber);
                    return true;
                } 
                // see if ??str
                else if(next == '?') {
                    stream.get();
                    next = stream.peek();
                    if(std::isalnum(next)) {
                        auto dat = getAlphaNumsUntilSpace(stream);
                        tokens.emplace_back(Lexeme::Q_Q_STRING, std::move(dat), lineNumber);
                        return true;
                    }
                }
            }
        } else if(c == '$') {
            auto next = stream.peek();
            if(next != EOF) {
                // see if $str
                if(std::isalnum(next)) {
                    auto dat = getAlphaNumsUntilSpace(stream);
                    tokens.emplace_back(Lexeme::DOLLAR_STRING, std::move(dat), lineNumber);
                    return true;
                }
            }
        }  else if(c == ';') {
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
                    if(com == '\n') {
                        ++lineNumber;
                    }
                    return true;
                }
            }
            tokens.emplace_back(Lexeme::SEMICOLON, ";", lineNumber);
            return true;
        }
        return false;
    }

    bool checkIfNumberToken(char const c,
                            std::istream & stream, 
                            std::vector<Token> & tokens,
                            long const lineNumber) {
        if(std::isdigit(c)) {
            std::string dat;
            dat.push_back(c);
            auto next = stream.peek();
            auto stillNeedDot = true;
            while(next != EOF && (std::isdigit(next) || (next == '.'))) {
                if(next == '.' && stillNeedDot) {
                    stillNeedDot = false;
                } else if(next == '.') {
                    break;
                }
                stream.get();
                dat.push_back(next);
                next = stream.peek();
            }
            // Found real number
            if(!stillNeedDot) {
                tokens.emplace_back(Lexeme::REAL_NUM, std::move(dat), lineNumber);
            } else {
                tokens.emplace_back(Lexeme::INTEGER_NUM, std::move(dat), lineNumber);
            }
            return true;
        }
        return false;
    }

    bool isPartOfEscapeCode(char const c) {
        return (c == 'a' || c == 'b' || c == 'f' ||
                c == 'n' || c == 'r' || c == 't' ||
                c == 'v' || c == '\\');
    }

    char getEscapeCode(char const c) {
        if(c == 'a') {
            return '\a';
        } else if(c == 'b') {
            return '\b';
        } else if(c == 'f') {
            return '\f';
        } else if(c == 'n') {
            return '\n';
        } else if(c == 'r') {
            return '\r';
        } else if(c == 't') {
            return '\t';
        } else if(c == 'v') {
            return '\v';
        } 
        return '\\';
    }

    bool checkIfLiteralStringToken(char const c,
                                   std::istream & stream,
                                   std::vector<Token> & tokens,
                                   long & lineNumber) {
        if(c == '"') {
            auto next = stream.peek();
            std::string dat;
            while (next != EOF && next != '"') {
                if(next == '\n') {
                    ++lineNumber;
                    break;
                } 
                // new line character handling
                else if(next == '\\') {
                    stream.get();
                    next = stream.peek();
                    if(isPartOfEscapeCode(next)) {
                        dat.push_back(getEscapeCode(next));
                        stream.get();
                        next = stream.peek();
                    }
                } else {
                    dat.push_back(next);
                    stream.get();
                    next = stream.peek();
                }
            }
            if(c != '"') {
                return false;
            }
            stream.get();
            tokens.emplace_back(Lexeme::LITERAL_STRING, std::move(dat), lineNumber);
        }
        return false;
    }

    /// Used for parsing program args in which we want to treat
    /// a string as it is without needing to require double quotes.
    bool checkIfRawStringTreatAsStringToken(char const c,
                                            std::istream & stream,
                                            std::vector<Token> & tokens,
                                            long & lineNumber) {
        auto next = stream.peek();
        std::string dat;
        dat.push_back(c);
        while (next != EOF && next != ' ') {
            if(next == '\n') {
                ++lineNumber;
                break;
            } 
            // new line character handling
            else if(next == '\\') {
                stream.get();
                next = stream.peek();
                if(isPartOfEscapeCode(next)) {
                    dat.push_back(getEscapeCode(next));
                    stream.get();
                    next = stream.peek();
                }
            } else {
                dat.push_back(next);
                stream.get();
                next = stream.peek();
            }
        }
        stream.get();
        tokens.emplace_back(Lexeme::LITERAL_STRING, std::move(dat), lineNumber);
        return false;
    }

    bool checkIfLiteralCharToken(char const c,
                                 std::istream & stream,
                                 std::vector<Token> & tokens,
                                 long & lineNumber) {
        if(c == '\'') {
            auto next = stream.peek();
            std::string dat;
            if(next != '\'') {

                // new-line char handling
                if(next == '\\') {
                    stream.get();
                    next = stream.peek();
                    if(isPartOfEscapeCode(next)) {
                        dat.push_back(getEscapeCode(next));
                        stream.get();
                    }
                } else {
                    dat.push_back(next);
                    stream.get();
                }
            }
            if(c != '\'') {
                return false;
            }
            stream.get();
            tokens.emplace_back(Lexeme::LITERAL_CHAR, std::move(dat), lineNumber);
        }
        return false;
    }

    std::vector<Token> Lexer::tokenize(std::istream & stream) {
        std::vector<Token> tokens;
        long lineNumber{1};
        char c;
        while(stream.get(c)) {
            if(c == ' ') { continue; }
            if(c == '\n') { ++lineNumber; continue; }
            if(checkIfDoubleCharToken(c, stream, tokens, lineNumber)) { continue; }
            if(checkIfSingleCharToken(c, tokens, lineNumber)) { continue; }
            if(checkIfMultiAlphaToken(c, stream, tokens, lineNumber)) { continue; }
            if(checkIfNumberToken(c, stream, tokens, lineNumber)) { continue; }
            if(checkIfLiteralStringToken(c, stream, tokens, lineNumber)) { continue; }
            if(checkIfLiteralCharToken(c, stream, tokens, lineNumber)) { continue; }
        }
        return tokens;
    }

    std::vector<Token> Lexer::tokenizeProgArgs(std::istream & stream) {
        std::vector<Token> tokens;
        long lineNumber{1};
        char c;
        while(stream.get(c)) {
            if(c == ' ') { continue; }
            if(c == '\n') { ++lineNumber; continue; }
            if(checkIfNumberToken(c, stream, tokens, lineNumber)) { continue; }
            if(checkIfRawStringTreatAsStringToken(c, stream, tokens, lineNumber)) { continue; }
        }
        return tokens;
    }

}