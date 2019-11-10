#pragma once

namespace jasl {
    enum class Lexeme {
        // Single char lexemes
        PLUS, MINUS, STAR, EQUAL, DIVIDE,
        COLON, SEMICOLON, OPEN_PAREN, CLOSE_PAREN,
        OPEN_SQUARE, CLOSE_SQUARE, OPEN_CURLY, CLOSE_CURLY,
        GREATER, LESS, HAT, QUESTION,

        // Double or tipple char lexemes
        NOT_EQUAL, GREATER_EQUAL, LESS_EQUAL, ARROW, COMMENT,
        EQUAL_EQUAL, QUESTION_QUESTION, HAT_HAT,

        // Literals
        GENERIC_STRING, LITERAL_STRING, INTEGER_NUM, REAL_NUM,
        HAT_STRING, HAT_HAT_STRING, Q_STRING, Q_Q_STRING,

        // Keywords
        FOR, EACH, REPEAT, TIMES, IF, START, FN, BOOL,
        INTEGER, REAL, STRING, IN, LIST, TRUE, FALSE,
        SAY, PR, PRN, INPUT

        // Built-ins
    };
}