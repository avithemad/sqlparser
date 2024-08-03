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
	invalid_tok = -8,
	and_tok = -9,
	or_tok = -10,
	pred_tok = -11,
	in_tok = -12,
	eq_tok = -13,
	like_tok = -14
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
	int getSrcIdx();
	std::string printDebug();
};
