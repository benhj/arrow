#pragma once

#include "Lexeme.hpp"
#include <string>
#include <utility>

namespace jasl {
	struct Token {

		Token(){}

		Token(Lexeme lex, std::string str) 
		: lexeme(std::move(lex))
		, raw(std::move(str))
		{
		}

		Lexeme lexeme;
		std::string raw;
	};
}