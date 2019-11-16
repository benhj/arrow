#include "Lexer.hpp"
#include "Lexeme.hpp"
#include <fstream>
#include <iostream>

int main(int argc, char ** argv) {

	std::ifstream in(argv[1]);

	auto tokens = jasl::Lexer::tokenize(in);

	for(auto const & token : tokens) {
		std::cout<<token.lexeme<<"\t"<<token.raw<<std::endl;
	}

	return 0;
}