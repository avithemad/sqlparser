#include "lexer.h"
#include "util.h"

Lexer::Lexer(const std::string &src) : src(src), src_idx(0) {
    eatToken();
}

Token Lexer::peekToken() {
    return cur_tok;
}

void Lexer::eatToken() {
    if(src_idx >= src.size()) {
        cur_tok = {eof_tok, ""};
        return;
    }
    // skip blank spaces
    while(isspace(src[src_idx])) {
        if(src_idx == src.size()) {
            cur_tok = {eof_tok, ""};
            return;
        }
        src_idx++;
    } 

    // now parse the tokens
    if (isalnum(src[src_idx])) {
        std::string lex;
        while(isalnum(src[src_idx])) {
            lex.push_back(src[src_idx++]);
        }
        if (stricmp("select", lex)) {
            cur_tok = {select_tok, lex};
        } else if (stricmp("from", lex)) {
            cur_tok = {from_tok, lex};
        } else if (stricmp("where", lex)) {
            cur_tok = {where_tok, lex};
        } else {
            cur_tok = {identifier_tok, lex};
        }
    } else if (src[src_idx] == '*') {
        cur_tok = {star_tok, "*"};
        src_idx++;
    } else if (src[src_idx] == ',') {
        cur_tok = {comma_tok, ","};
        src_idx++;
    } else {
        cur_tok = {invalid_tok, ""};
    }
}