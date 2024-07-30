#include "lexer.h"
#include "parser.h"
#include<iostream>

int main() {
    // Lexer lex("select a, b from t1, t2");
    // Token tok;
    // while((tok=lex.peekToken()).type != eof_tok && tok.type != invalid_tok) {
    //     std::cout << tok.type << " " << tok.lexeme << std::endl;
    //     lex.eatToken();
    // }

    Parser parser("select a, b from t1, t2");
    auto sq = parser.ParseSelectQuery();
    if (sq == nullptr) std::cout << "Incorrect syntax\n";
    else std::cout << "Parsed successfully\n";
}