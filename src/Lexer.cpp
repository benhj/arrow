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
            case '?':
            {
                auto next = stream.peek();
                if(next != EOF && char(next) == '?') {
                    stream.get(); // iterate
                    tokens.emplace_back(Lexeme::QUESTION_QUESTION, "??");
                    return true;
                }
                tokens.emplace_back(Lexeme::QUESTION, "?");
                return false;
            }
            case '^':
            {
                auto next = stream.peek();
                if(next != EOF && char(next) == '^') {
                    stream.get(); // iterate
                    tokens.emplace_back(Lexeme::HAT_HAT, "^^");
                    return true;
                }
                tokens.emplace_back(Lexeme::HAT, "^");
                return false;
            }
            default: return false;
        }
        return false;
    }

    bool checkIfSingleCharToken(char const c, std::vector<Token> & tokens) {
        switch (c) {
            case '+': tokens.emplace_back(Lexeme::ARROW, "+"); break;
            case '*': tokens.emplace_back(Lexeme::STAR, "*"); break;
            case '=': tokens.emplace_back(Lexeme::EQUAL, "="); break;
            case '{': tokens.emplace_back(Lexeme::OPEN_CURLY, "{"); break;
            case '}': tokens.emplace_back(Lexeme::CLOSE_CURLY, "}"); break;
            case '[': tokens.emplace_back(Lexeme::OPEN_SQUARE, "}"); break;
            case ']': tokens.emplace_back(Lexeme::CLOSE_SQUARE, "}"); break;
            case '(': tokens.emplace_back(Lexeme::OPEN_PAREN, "}"); break;
            case ')': tokens.emplace_back(Lexeme::CLOSE_PAREN, "}"); break;
            default : return false;
        };
        return true;
    }

    bool checkIfMultiAlphaToken(char const c, std::istream & stream, std::vector<Token> & tokens) {
        // check if beginning of 'generic string'
        if(std::isalpha(c)) {
            std::string dat;
            dat.push_back(c);
            auto next = stream.peek();
            while (next != EOF && std::isalpha(next)) {
                stream.get(); // iterate;
                dat.push_back(c);
                next = stream.peek();
            }
            tokens.emplace_back(Lexeme::GENERIC_STRING, dat);
            return true;
        }
        return false;
    }

    std::vector<Token> Lexer::tokenize(std::istream & stream) const {
        std::vector<Token> tokens;
        char c;
        while(stream.get(c)) {
            if(checkIfDoubleCharToken(c, stream, tokens)) { continue; }
            if(checkIfSingleCharToken(c, tokens)) { continue; }
            if(checkIfMultiAlphaToken(c, stream, tokens)) { continue; }
        } 
    }

}