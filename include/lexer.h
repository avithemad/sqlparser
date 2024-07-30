#pragma once
#include<string>

enum token_type {
	select_tok = -1,
	from_tok = -2,
	star_tok = -3,
	comma_tok = -4,
	identifier_tok = -5,
	where_tok = -6,
	eof_tok = -7,
	invalid_tok = -8
};
	

struct Token {
	token_type type;
	std::string lexeme;
};

class Lexer {
	const std::string src;
	int src_idx;
	Token cur_tok;
public:
	Lexer(const std::string &src);
	Token peekToken();
	void eatToken();
};
