#include "Lexeme.hpp"
#include <map>
#include <string>
#include <vector>

namespace jasl {
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
    #undef INSERT_ELEMENT
        }
        return std::find_if(std::begin(strings), std::end(strings),
                        [&lexeme](Lexeme lex) {
                            return lex == lexeme;
                        }) != std::end(strings);
    }
}

std::ostream& operator<<(std::ostream& out, jasl::Lexeme const value){
    static std::map<jasl::Lexeme, std::string> strings;
    if (strings.size() == 0) {
#define INSERT_ELEMENT(p) strings[p] = #p
        INSERT_ELEMENT(jasl::Lexeme::PLUS);  
        INSERT_ELEMENT(jasl::Lexeme::MINUS);
        INSERT_ELEMENT(jasl::Lexeme::STAR);
        INSERT_ELEMENT(jasl::Lexeme::EQUAL);
        INSERT_ELEMENT(jasl::Lexeme::DIVIDE);
        INSERT_ELEMENT(jasl::Lexeme::COLON);
        INSERT_ELEMENT(jasl::Lexeme::SEMICOLON);
        INSERT_ELEMENT(jasl::Lexeme::OPEN_PAREN);
        INSERT_ELEMENT(jasl::Lexeme::CLOSE_PAREN);
        INSERT_ELEMENT(jasl::Lexeme::OPEN_SQUARE);
        INSERT_ELEMENT(jasl::Lexeme::CLOSE_SQUARE);
        INSERT_ELEMENT(jasl::Lexeme::OPEN_CURLY);
        INSERT_ELEMENT(jasl::Lexeme::CLOSE_CURLY);
        INSERT_ELEMENT(jasl::Lexeme::GREATER);
        INSERT_ELEMENT(jasl::Lexeme::LESS);
        INSERT_ELEMENT(jasl::Lexeme::HAT);
        INSERT_ELEMENT(jasl::Lexeme::QUESTION);
        INSERT_ELEMENT(jasl::Lexeme::COMMA);

        INSERT_ELEMENT(jasl::Lexeme::NOT_EQUAL);
        INSERT_ELEMENT(jasl::Lexeme::GREATER_EQUAL);
        INSERT_ELEMENT(jasl::Lexeme::LESS_EQUAL);
        INSERT_ELEMENT(jasl::Lexeme::ARROW);
        INSERT_ELEMENT(jasl::Lexeme::COMMENT);
        INSERT_ELEMENT(jasl::Lexeme::EQUAL_EQUAL);
        INSERT_ELEMENT(jasl::Lexeme::QUESTION_QUESTION);
        INSERT_ELEMENT(jasl::Lexeme::HAT_HAT);

        INSERT_ELEMENT(jasl::Lexeme::GENERIC_STRING);
        INSERT_ELEMENT(jasl::Lexeme::LITERAL_STRING);
        INSERT_ELEMENT(jasl::Lexeme::INTEGER_NUM);
        INSERT_ELEMENT(jasl::Lexeme::REAL_NUM);
        INSERT_ELEMENT(jasl::Lexeme::HAT_STRING);
        INSERT_ELEMENT(jasl::Lexeme::HAT_HAT_STRING);
        INSERT_ELEMENT(jasl::Lexeme::Q_STRING);
        INSERT_ELEMENT(jasl::Lexeme::Q_Q_STRING);

        INSERT_ELEMENT(jasl::Lexeme::FOR);
        INSERT_ELEMENT(jasl::Lexeme::EACH);
        INSERT_ELEMENT(jasl::Lexeme::REPEAT);  
        INSERT_ELEMENT(jasl::Lexeme::TIMES); 
        INSERT_ELEMENT(jasl::Lexeme::IF);
        INSERT_ELEMENT(jasl::Lexeme::START); 
        INSERT_ELEMENT(jasl::Lexeme::FN); 
        INSERT_ELEMENT(jasl::Lexeme::BOOL);
        INSERT_ELEMENT(jasl::Lexeme::INTEGER);
        INSERT_ELEMENT(jasl::Lexeme::REAL);
        INSERT_ELEMENT(jasl::Lexeme::STRING);
        INSERT_ELEMENT(jasl::Lexeme::IN);
        INSERT_ELEMENT(jasl::Lexeme::LIST);
        INSERT_ELEMENT(jasl::Lexeme::TRUE);
        INSERT_ELEMENT(jasl::Lexeme::FALSE);
        INSERT_ELEMENT(jasl::Lexeme::SAY);
        INSERT_ELEMENT(jasl::Lexeme::PR);
        INSERT_ELEMENT(jasl::Lexeme::PRN);
        INSERT_ELEMENT(jasl::Lexeme::INPUT);
        INSERT_ELEMENT(jasl::Lexeme::WHILE);
        INSERT_ELEMENT(jasl::Lexeme::PUT);
        INSERT_ELEMENT(jasl::Lexeme::ELSE);
        INSERT_ELEMENT(jasl::Lexeme::CALL);
#undef INSERT_ELEMENT
    }   

    return out << strings[value];
}