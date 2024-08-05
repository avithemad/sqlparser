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
    char c;
    std::string sqlsrc;
    while((c = getchar())!=EOF) {
        sqlsrc.push_back(c);
    }
    Parser parser(sqlsrc);
    auto sq = parser.ParseQuery();
    if (sq == nullptr) std::cout << "Incorrect syntax\n";
    else std::cout << "Parsed successfully\n";
    auto ra = sq->LowerToRelalg();
    if (ra == nullptr) std::cout << "Could not lower to relational algebra\n";
    else {
        std::cout << "Lowered to relational algebra successfully\n";
        std::cout << ra->print();
    }
}