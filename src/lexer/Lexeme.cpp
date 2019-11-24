#include "Lexeme.hpp"
#include <map>
#include <string>
#include <vector>

namespace arrow {
    bool isOperator(Lexeme const lexeme)
    {
        static std::vector<Lexeme> strings;
        if (strings.size() == 0) {
    #define INSERT_ELEMENT(p) strings.push_back(p);
            INSERT_ELEMENT(Lexeme::PLUS);  
            INSERT_ELEMENT(Lexeme::MINUS);
            INSERT_ELEMENT(Lexeme::STAR);
            INSERT_ELEMENT(Lexeme::EQUAL);
            INSERT_ELEMENT(Lexeme::DIVIDE);
            INSERT_ELEMENT(Lexeme::GREATER);
            INSERT_ELEMENT(Lexeme::LESS);
            INSERT_ELEMENT(Lexeme::NOT_EQUAL);
            INSERT_ELEMENT(Lexeme::GREATER_EQUAL);
            INSERT_ELEMENT(Lexeme::LESS_EQUAL);
            INSERT_ELEMENT(Lexeme::AND);
            INSERT_ELEMENT(Lexeme::OR);
    #undef INSERT_ELEMENT
        }
        return std::find_if(std::begin(strings), std::end(strings),
                        [&lexeme](Lexeme lex) {
                            return lex == lexeme;
                        }) != std::end(strings);
    }

    bool isMathOperator(Lexeme const lexeme)
    {
        static std::vector<Lexeme> strings;
        if (strings.size() == 0) {
    #define INSERT_ELEMENT(p) strings.push_back(p);
            INSERT_ELEMENT(Lexeme::PLUS);  
            INSERT_ELEMENT(Lexeme::MINUS);
            INSERT_ELEMENT(Lexeme::STAR);
            INSERT_ELEMENT(Lexeme::DIVIDE);
    #undef INSERT_ELEMENT
        }
        return std::find_if(std::begin(strings), std::end(strings),
                        [&lexeme](Lexeme lex) {
                            return lex == lexeme;
                        }) != std::end(strings);
    }

    bool isBooleanOperator(Lexeme const lexeme)
    {
        static std::vector<Lexeme> strings;
        if (strings.size() == 0) {
    #define INSERT_ELEMENT(p) strings.push_back(p);
            INSERT_ELEMENT(Lexeme::EQUAL);
            INSERT_ELEMENT(Lexeme::GREATER);
            INSERT_ELEMENT(Lexeme::LESS);
            INSERT_ELEMENT(Lexeme::NOT_EQUAL);
            INSERT_ELEMENT(Lexeme::GREATER_EQUAL);
            INSERT_ELEMENT(Lexeme::LESS_EQUAL);
            INSERT_ELEMENT(Lexeme::AND);
            INSERT_ELEMENT(Lexeme::OR);
    #undef INSERT_ELEMENT
        }
        return std::find_if(std::begin(strings), std::end(strings),
                        [&lexeme](Lexeme lex) {
                            return lex == lexeme;
                        }) != std::end(strings);
    }
}

std::ostream& operator<<(std::ostream& out, arrow::Lexeme const value){
    static std::map<arrow::Lexeme, std::string> strings;
    if (strings.size() == 0) {
#define INSERT_ELEMENT(p) strings[p] = #p
        INSERT_ELEMENT(arrow::Lexeme::PLUS);  
        INSERT_ELEMENT(arrow::Lexeme::MINUS);
        INSERT_ELEMENT(arrow::Lexeme::STAR);
        INSERT_ELEMENT(arrow::Lexeme::EQUAL);
        INSERT_ELEMENT(arrow::Lexeme::DIVIDE);
        INSERT_ELEMENT(arrow::Lexeme::COLON);
        INSERT_ELEMENT(arrow::Lexeme::SEMICOLON);
        INSERT_ELEMENT(arrow::Lexeme::OPEN_PAREN);
        INSERT_ELEMENT(arrow::Lexeme::CLOSE_PAREN);
        INSERT_ELEMENT(arrow::Lexeme::OPEN_SQUARE);
        INSERT_ELEMENT(arrow::Lexeme::CLOSE_SQUARE);
        INSERT_ELEMENT(arrow::Lexeme::OPEN_CURLY);
        INSERT_ELEMENT(arrow::Lexeme::CLOSE_CURLY);
        INSERT_ELEMENT(arrow::Lexeme::GREATER);
        INSERT_ELEMENT(arrow::Lexeme::LESS);
        INSERT_ELEMENT(arrow::Lexeme::COMMA);

        INSERT_ELEMENT(arrow::Lexeme::NOT_EQUAL);
        INSERT_ELEMENT(arrow::Lexeme::GREATER_EQUAL);
        INSERT_ELEMENT(arrow::Lexeme::LESS_EQUAL);
        INSERT_ELEMENT(arrow::Lexeme::ARROW);
        INSERT_ELEMENT(arrow::Lexeme::COMMENT);
        INSERT_ELEMENT(arrow::Lexeme::EQUAL_EQUAL);
        INSERT_ELEMENT(arrow::Lexeme::AND);
        INSERT_ELEMENT(arrow::Lexeme::OR);

        INSERT_ELEMENT(arrow::Lexeme::GENERIC_STRING);
        INSERT_ELEMENT(arrow::Lexeme::LITERAL_STRING);
        INSERT_ELEMENT(arrow::Lexeme::INTEGER_NUM);
        INSERT_ELEMENT(arrow::Lexeme::REAL_NUM);
        INSERT_ELEMENT(arrow::Lexeme::HAT_STRING);
        INSERT_ELEMENT(arrow::Lexeme::HAT_HAT_STRING);
        INSERT_ELEMENT(arrow::Lexeme::Q_STRING);
        INSERT_ELEMENT(arrow::Lexeme::Q_Q_STRING);

        INSERT_ELEMENT(arrow::Lexeme::FOR);
        INSERT_ELEMENT(arrow::Lexeme::EACH);
        INSERT_ELEMENT(arrow::Lexeme::REPEAT);  
        INSERT_ELEMENT(arrow::Lexeme::TIMES); 
        INSERT_ELEMENT(arrow::Lexeme::IF);
        INSERT_ELEMENT(arrow::Lexeme::START); 
        INSERT_ELEMENT(arrow::Lexeme::FN); 
        INSERT_ELEMENT(arrow::Lexeme::BOOL);
        INSERT_ELEMENT(arrow::Lexeme::INTEGER);
        INSERT_ELEMENT(arrow::Lexeme::REAL);
        INSERT_ELEMENT(arrow::Lexeme::STRING);
        INSERT_ELEMENT(arrow::Lexeme::IN);
        INSERT_ELEMENT(arrow::Lexeme::LIST);
        INSERT_ELEMENT(arrow::Lexeme::TRUE);
        INSERT_ELEMENT(arrow::Lexeme::FALSE);
        INSERT_ELEMENT(arrow::Lexeme::SAY);
        INSERT_ELEMENT(arrow::Lexeme::PR);
        INSERT_ELEMENT(arrow::Lexeme::PRN);
        INSERT_ELEMENT(arrow::Lexeme::INPUT);
        INSERT_ELEMENT(arrow::Lexeme::WHILE);
        INSERT_ELEMENT(arrow::Lexeme::PUT);
        INSERT_ELEMENT(arrow::Lexeme::ELSE);
        INSERT_ELEMENT(arrow::Lexeme::CALL);
#undef INSERT_ELEMENT
    }   

    return out << strings[value];
}